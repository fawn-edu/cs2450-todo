#include <algorithm>
#include <cassert>
#include <format>
#include <fstream>
#include <iostream>
#include <print>
#include <vector>

#include <imgui.h>
#include <imgui_impl_sdl3.h>
#include <imgui_impl_sdlgpu3.h>
#include "vendor/imgui/misc/cpp/imgui_stdlib.h"
#include <SDL3/SDL.h>

#include "task.hpp"
#include "task_controller2.hpp"
#include "vendor/imgui/imgui.h"

static int FilterAlphabetic(ImGuiInputTextCallbackData* data) {
	return 'z' < data->EventChar || data->EventChar < 'A' ? 1 : 0;
}

int main() {
	std::vector<task> tasks{};
	std::ifstream itodo{"todo.txt", std::ios::in | std::ios::app};
	if (!itodo.is_open()) {
		std::println(std::cerr, "could not open todo.txt for reading");
		return 1;
	}
	auto linenr = 1u;
	for (std::string line; getline(itodo, line); ++linenr) {
		try {
			tasks.emplace_back(task::parse(line));
		} catch (std::invalid_argument &e) {
			std::println(std::cerr, "syntax error in todo.txt on line {}: {}", linenr, e.what());
			return 1;
		}
	}

	task_controller c{tasks};

	// TODO: Move all the GUI things to task_view
	// Setup SDL
	if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMEPAD)) {
		std::println(std::cerr, "Error: SDL_Init(): {}", SDL_GetError());
		return 1;
	}

	// Create SDL window graphics context
	float main_scale = SDL_GetDisplayContentScale(SDL_GetPrimaryDisplay());
	SDL_WindowFlags window_flags = SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIDDEN | SDL_WINDOW_HIGH_PIXEL_DENSITY;
	SDL_Window* window = SDL_CreateWindow("CS 2450 Task Manager", (int)(1280 * main_scale), (int)(800 * main_scale), window_flags);
	if (!window) {
		std::println(std::cerr, "Error: SDL_CreateWindow(): {}", SDL_GetError());
		return 1;
	}
	SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
	SDL_ShowWindow(window);

	// Create GPU Device
	SDL_GPUDevice* gpu_device = SDL_CreateGPUDevice(SDL_GPU_SHADERFORMAT_SPIRV | SDL_GPU_SHADERFORMAT_DXIL | SDL_GPU_SHADERFORMAT_MSL | SDL_GPU_SHADERFORMAT_METALLIB, true, nullptr);
	if (!gpu_device) {
		std::println(std::cerr, "Error: SDL_CreateGPUDevice(): {}", SDL_GetError());
		return 1;
	}

	// Claim window for GPU Device
	if (!SDL_ClaimWindowForGPUDevice(gpu_device, window)) {
		std::println(std::cerr, "Error: SDL_ClaimWindowForGPUDevice(): {}", SDL_GetError());
		return 1;
	}
	SDL_SetGPUSwapchainParameters(gpu_device, window, SDL_GPU_SWAPCHAINCOMPOSITION_SDR, SDL_GPU_PRESENTMODE_VSYNC);

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad; // Enable Gamepad Controls

	// Setup Dear ImGui style
	ImGuiStyle& style = ImGui::GetStyle();
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsLight();
	//style.FontSizeBase = 16.0f; // Default to 13.0f when unspecified, which allows crisp legacy font for non-4K screens.
	style.FrameRounding = 3.0f;
	style.GrabRounding = 3.0f;

	// Setup scaling
	style.ScaleAllSizes(main_scale); // Bake a fixed style scale. (until we have a solution for dynamic style scaling, changing this requires resetting Style + calling this again)
	style.FontScaleDpi = main_scale; // Set initial font scale

	// Setup Platform/Renderer backends
	ImGui_ImplSDL3_InitForSDLGPU(window);
	ImGui_ImplSDLGPU3_InitInfo init_info = {};
	init_info.Device = gpu_device;
	init_info.ColorTargetFormat = SDL_GetGPUSwapchainTextureFormat(gpu_device, window);
	init_info.MSAASamples = SDL_GPU_SAMPLECOUNT_1; // Only used in multi-viewports mode.
	init_info.SwapchainComposition = SDL_GPU_SWAPCHAINCOMPOSITION_SDR; // Only used in multi-viewports mode.
	init_info.PresentMode = SDL_GPU_PRESENTMODE_VSYNC;
	ImGui_ImplSDLGPU3_Init(&init_info);

	for (bool done = false; !done;) {
		// Poll and handle events (inputs, window resize, etc.)
		// You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
		// - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application, or clear/overwrite your copy of the mouse data.
		// - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application, or clear/overwrite your copy of the keyboard data.
		// Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
		for (SDL_Event event; SDL_PollEvent(&event);) {
			ImGui_ImplSDL3_ProcessEvent(&event);
			if (event.type == SDL_EVENT_QUIT) done = true;
			if (event.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED && event.window.windowID == SDL_GetWindowID(window)) done = true;
		}

		if (SDL_GetWindowFlags(window) & SDL_WINDOW_MINIMIZED) {
			SDL_Delay(10);
			continue;
		}

		// Start the Dear ImGui frame
		ImGui_ImplSDLGPU3_NewFrame();
		ImGui_ImplSDL3_NewFrame();
		ImGui::NewFrame();

			static bool yes = true;
			// ImGui::ShowDemoWindow(&yes);
		{
			static ImGuiWindowFlags flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings;

			const ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->Pos);
			ImGui::SetNextWindowSize(viewport->Size);
			ImGui::Begin("Task list", &yes, flags);

			static std::string edit_name{};
			static char priority[2];

			if (ImGui::Button("New task")) {
				edit_name.clear();
				*priority = 0;
				ImGui::OpenPopup("Create task");
			}

			ImVec2 center = ImGui::GetMainViewport()->GetCenter();
			ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
			ImGui::PushStyleColor(ImGuiCol_PopupBg, ImVec4(0.18f, 0.18f, 0.18f, 1.00f));
			if (ImGui::BeginPopupModal("Create task", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
				ImGui::InputTextWithHint("Task name", "Shave yaks", &edit_name);
				ImGui::InputText("Task priority (optional, A-Z)", priority, IM_COUNTOF(priority), ImGuiInputTextFlags_CallbackCharFilter, FilterAlphabetic);

				if (ImGui::Button("Save", ImVec2(120, 0))) {
					if (edit_name.empty()) edit_name = "New task";
					auto t = task{.name = edit_name};
					if (*priority) {
						t.priority.emplace(static_cast<char>(std::toupper(*priority)));
					};
					tasks.emplace_back(std::move(t)); // TODO: this needs to be a method on task_controller
					ImGui::CloseCurrentPopup();
				}
				ImGui::SetItemDefaultFocus();
				ImGui::SameLine();
				if (ImGui::Button("Cancel", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
				ImGui::EndPopup();
			}
			ImGui::PopStyleColor(1);
			
			const auto list_tasks = [&](auto cond) {
				ImGui::PushStyleColor(ImGuiCol_PopupBg, ImVec4(0.18f, 0.18f, 0.18f, 1.00f));
				if (ImGui::BeginTable("tasks", 2, ImGuiTableFlags_SizingStretchProp)) {
					for (auto i = 0u; i < tasks.size(); ++i) {
						auto &t = tasks[i];
						if (!cond(t)) continue;
						ImGui::TableNextRow();
						ImGui::PushID(i);
						bool complete = t.completed.has_value();
						ImGui::TableSetColumnIndex(0);
						ImGui::Checkbox("", &complete);
						ImGui::SameLine();
						ImGui::Text("[%c] %s", t.priority.value_or(' '), t.name.c_str());

						ImGui::TableSetColumnIndex(1);

						ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0.13f, 0.6f, 0.6f));
						ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(0.13f, 0.7f, 0.7f));
						ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(0.13f, 0.8f, 0.8f));
						ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
						if (ImGui::Button("Edit")) {
							edit_name = t.name;
							*priority = t.priority.value_or('\0');
							ImGui::OpenPopup("Edit task");
						}
						ImGui::PopStyleColor(4);

						ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
						if (ImGui::BeginPopupModal("Edit task", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
							ImGui::InputTextWithHint("Task name", "Shave yaks", &edit_name);
							ImGui::InputText("Task priority (optional, A-Z)", priority, IM_COUNTOF(priority), ImGuiInputTextFlags_CallbackCharFilter, FilterAlphabetic);

							// TODO: Is it possible to move this into task_controller?
							// It needs to live there to idomatically satisfy MVC architecture
							if (ImGui::Button("Save", ImVec2(120, 0))) {
								t.name = edit_name;
								if (*priority) {
									t.priority.emplace(static_cast<char>(std::toupper(*priority)));
								} else t.priority = {};
								ImGui::CloseCurrentPopup();
							}
							ImGui::SetItemDefaultFocus();
							ImGui::SameLine();
							if (ImGui::Button("Cancel", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
							ImGui::EndPopup();
						}

						ImGui::SameLine();

						ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0, 0.6f, 0.6f));
						ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(0, 0.7f, 0.7f));
						ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(0, 0.8f, 0.8f));
						ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
						if (ImGui::Button("Delete")) ImGui::OpenPopup("Delete task");
						ImGui::PopStyleColor(4);

						ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
						if (ImGui::BeginPopupModal("Delete task", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
							ImGui::Text("Delete `%s`?\nThis operation cannot be undone!", t.name.c_str());
							ImGui::Separator();

							ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0, 0.6f, 0.6f));
							ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(0, 0.7f, 0.7f));
							ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(0, 0.8f, 0.8f));
							ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
							if (ImGui::Button("Delete", ImVec2(120, 0))) {
								// TODO: task_controller?
								tasks.erase(tasks.begin() + std::distance(tasks.data(), &tasks[i]));
								ImGui::CloseCurrentPopup();
							}
							ImGui::PopStyleColor(4);

							ImGui::SetItemDefaultFocus();
							ImGui::SameLine();
							if (ImGui::Button("Cancel", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
							ImGui::EndPopup();
						}
						
						ImGui::PopID();

						if (complete) t.complete(); else t.completed = {};
					}
					ImGui::EndTable();
					std::ranges::sort(tasks);
				}
				ImGui::PopStyleColor(1);
			};

			ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_None;
			if (ImGui::BeginTabBar("TasksTabs", tab_bar_flags)) {
				if (ImGui::BeginTabItem("All")) {
					list_tasks([](const auto &){ return true; });
					ImGui::EndTabItem();
				}
				if (ImGui::BeginTabItem("Incomplete")) {
					list_tasks([](const auto &t){ return !t.completed.has_value(); });
					ImGui::EndTabItem();
				}
				if (ImGui::BeginTabItem("Complete")) {
					list_tasks([](const auto &t){ return t.completed.has_value(); });
					ImGui::EndTabItem();
				}
				ImGui::EndTabBar();
			}
			ImGui::End();
		}

		// Rendering
		ImGui::Render();
		ImDrawData* draw_data = ImGui::GetDrawData();
		const bool is_minimized = (draw_data->DisplaySize.x <= 0.0f || draw_data->DisplaySize.y <= 0.0f);

		SDL_GPUCommandBuffer* command_buffer = SDL_AcquireGPUCommandBuffer(gpu_device); // Acquire a GPU command buffer

		SDL_GPUTexture* swapchain_texture;
		SDL_WaitAndAcquireGPUSwapchainTexture(command_buffer, window, &swapchain_texture, nullptr, nullptr); // Acquire a swapchain texture

		if (swapchain_texture != nullptr && !is_minimized)
		{
			// This is mandatory: call ImGui_ImplSDLGPU3_PrepareDrawData() to upload the vertex/index buffer!
			ImGui_ImplSDLGPU3_PrepareDrawData(draw_data, command_buffer);

			// Setup and start a render pass
			SDL_GPUColorTargetInfo target_info = {};
			target_info.texture = swapchain_texture;
			target_info.load_op = SDL_GPU_LOADOP_CLEAR;
			target_info.store_op = SDL_GPU_STOREOP_STORE;
			target_info.mip_level = 0;
			target_info.layer_or_depth_plane = 0;
			target_info.cycle = false;
			SDL_GPURenderPass* render_pass = SDL_BeginGPURenderPass(command_buffer, &target_info, 1, nullptr);

			// Render ImGui
			ImGui_ImplSDLGPU3_RenderDrawData(draw_data, command_buffer, render_pass);

			SDL_EndGPURenderPass(render_pass);
		}

		// Submit the command buffer
		SDL_SubmitGPUCommandBuffer(command_buffer);
	}

	// Cleanup
	SDL_WaitForGPUIdle(gpu_device);
	ImGui_ImplSDL3_Shutdown();
	ImGui_ImplSDLGPU3_Shutdown();
	ImGui::DestroyContext();

	SDL_ReleaseWindowFromGPUDevice(gpu_device, window);
	SDL_DestroyGPUDevice(gpu_device);
	SDL_DestroyWindow(window);
	SDL_Quit();

	// Save tasks to todo.txt
	std::ofstream otodo{"todo.txt", std::ios::out | std::ios::trunc};
	if (!otodo.is_open()) {
		std::println(std::cerr, "could not open todo.txt for writing");
		return 1;
	}
	for (const auto &task : tasks) std::println(otodo, "{}", task);
	return 0;
}
