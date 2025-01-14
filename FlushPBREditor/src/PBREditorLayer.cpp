#include "PBREditorLayer.h"

#include "ImGui/ImGuizmo.h"

namespace Flush {

	static void ImGuiShowHelpMarker(const char* desc)
	{
		ImGui::TextDisabled("(?)");
		if (ImGui::IsItemHovered())
		{
			ImGui::BeginTooltip();
			ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
			ImGui::TextUnformatted(desc);
			ImGui::PopTextWrapPos();
			ImGui::EndTooltip();
		}
	}

	PBREditorLayer::PBREditorLayer()
		: m_Scene(Scene::Model), m_Camera(glm::perspectiveFov(glm::radians(45.0f), 1280.0f, 720.0f, 0.1f, 10000.0f))
	{
	}

	PBREditorLayer::~PBREditorLayer()
	{
	}
	// todo: 初始化 mesh,material,shader,renderPass[geoPass,finalPass]
	void PBREditorLayer::OnAttach()
	{
#pragma region ImGui Color 初始化
		ImVec4* colors = ImGui::GetStyle().Colors;
		colors[ImGuiCol_Text] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
		colors[ImGuiCol_TextDisabled] = ImVec4(0.5f, 0.5f, 0.5f, 1.0f);
		colors[ImGuiCol_WindowBg] = ImVec4(0.18f, 0.18f, 0.18f, 1.0f); // Window background
		colors[ImGuiCol_ChildBg] = ImVec4(1.0f, 1.0f, 1.0f, 0.0f);
		colors[ImGuiCol_PopupBg] = ImVec4(0.08f, 0.08f, 0.08f, 0.94f);
		colors[ImGuiCol_Border] = ImVec4(0.43f, 0.43f, 0.50f, 0.5f);
		colors[ImGuiCol_BorderShadow] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
		colors[ImGuiCol_FrameBg] = ImVec4(0.3f, 0.3f, 0.3f, 0.5f); // Widget backgrounds
		colors[ImGuiCol_FrameBgHovered] = ImVec4(0.4f, 0.4f, 0.4f, 0.4f);
		colors[ImGuiCol_FrameBgActive] = ImVec4(0.4f, 0.4f, 0.4f, 0.6f);
		colors[ImGuiCol_TitleBg] = ImVec4(0.04f, 0.04f, 0.04f, 1.0f);
		colors[ImGuiCol_TitleBgActive] = ImVec4(0.29f, 0.29f, 0.29f, 1.0f);
		colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.0f, 0.0f, 0.0f, 0.51f);
		colors[ImGuiCol_MenuBarBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.0f);
		colors[ImGuiCol_ScrollbarBg] = ImVec4(0.02f, 0.02f, 0.02f, 0.53f);
		colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.31f, 0.31f, 0.31f, 1.0f);
		colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.41f, 0.41f, 0.41f, 1.0f);
		colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.51f, 0.51f, 0.51f, 1.0f);
		colors[ImGuiCol_CheckMark] = ImVec4(0.94f, 0.94f, 0.94f, 1.0f);
		colors[ImGuiCol_SliderGrab] = ImVec4(0.51f, 0.51f, 0.51f, 0.7f);
		colors[ImGuiCol_SliderGrabActive] = ImVec4(0.66f, 0.66f, 0.66f, 1.0f);
		colors[ImGuiCol_Button] = ImVec4(0.44f, 0.44f, 0.44f, 0.4f);
		colors[ImGuiCol_ButtonHovered] = ImVec4(0.46f, 0.47f, 0.48f, 1.0f);
		colors[ImGuiCol_ButtonActive] = ImVec4(0.42f, 0.42f, 0.42f, 1.0f);
		colors[ImGuiCol_Header] = ImVec4(0.7f, 0.7f, 0.7f, 0.31f);
		colors[ImGuiCol_HeaderHovered] = ImVec4(0.7f, 0.7f, 0.7f, 0.8f);
		colors[ImGuiCol_HeaderActive] = ImVec4(0.48f, 0.5f, 0.52f, 1.0f);
		colors[ImGuiCol_Separator] = ImVec4(0.43f, 0.43f, 0.5f, 0.5f);
		colors[ImGuiCol_SeparatorHovered] = ImVec4(0.72f, 0.72f, 0.72f, 0.78f);
		colors[ImGuiCol_SeparatorActive] = ImVec4(0.51f, 0.51f, 0.51f, 1.0f);
		colors[ImGuiCol_ResizeGrip] = ImVec4(0.91f, 0.91f, 0.91f, 0.25f);
		colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.81f, 0.81f, 0.81f, 0.67f);
		colors[ImGuiCol_ResizeGripActive] = ImVec4(0.46f, 0.46f, 0.46f, 0.95f);
		colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.0f);
		colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.0f, 0.43f, 0.35f, 1.0f);
		colors[ImGuiCol_PlotHistogram] = ImVec4(0.73f, 0.6f, 0.15f, 1.0f);
		colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.0f, 0.6f, 0.0f, 1.0f);
		colors[ImGuiCol_TextSelectedBg] = ImVec4(0.87f, 0.87f, 0.87f, 0.35f);
		colors[ImGuiCol_ModalWindowDarkening] = ImVec4(0.8f, 0.8f, 0.8f, 0.35f);
		colors[ImGuiCol_DragDropTarget] = ImVec4(1.0f, 1.0f, 0.0f, 0.9f);
		colors[ImGuiCol_NavHighlight] = ImVec4(0.60f, 0.6f, 0.6f, 1.0f);
		colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.0f, 1.0f, 1.0f, 0.7f);
#pragma endregion 
		using namespace glm;
#pragma region 缓存本地mesh,material,shader

		m_Mesh.reset(new Mesh("assets/models/m1911/m1911.fbx"));
		m_MeshMaterial.reset(new MaterialInstance(m_Mesh->GetMaterial()));

		m_QuadShader = Shader::Create("assets/shaders/quad.glsl");
		m_HDRShader = Shader::Create("assets/shaders/hdr.glsl");

		m_SphereMesh.reset(new Mesh("assets/models/Sphere1m.fbx"));
		m_PlaneMesh.reset(new Mesh("assets/models/Plane1m.obj"));

		m_GridShader = Shader::Create("assets/shaders/Grid.glsl");
		m_GridMaterial = MaterialInstance::Create(Material::Create(m_GridShader));
		m_GridMaterial->Set("u_Scale", m_GridScale);
		m_GridMaterial->Set("u_Res", m_GridSize);

		// Editor
		m_CheckerboardTex.reset(Texture2D::Create("assets/editor/Checkerboard.tga"));

		// Environment
		m_EnvironmentCubeMap.reset(TextureCube::Create("assets/textures/environments/Arches_E_PineTree_Radiance.tga"));
		//m_EnvironmentCubeMap.reset(TextureCube::Create("assets/textures/environments/DebugCubeMap.tga"));
		m_EnvironmentIrradiance.reset(TextureCube::Create("assets/textures/environments/Arches_E_PineTree_Irradiance.tga"));
		m_BRDFLUT.reset(Texture2D::Create("assets/textures/BRDF_LUT.tga"));
#pragma endregion 

#pragma region 设置2个Framebuffer,geoPassBuffer,finalScreenFrameBuffer
		// Render Passes
		FramebufferSpecification geoFramebufferSpec;
		geoFramebufferSpec.Width = 1280;
		geoFramebufferSpec.Height = 720;
		geoFramebufferSpec.Format = FramebufferFormat::RGBA16F;
		geoFramebufferSpec.ClearColor = { 0.1f, 0.1f, 0.1f, 1.0f };

		RenderPassSpecification geoRenderPassSpec;
		geoRenderPassSpec.TargetFramebuffer = Flush::Framebuffer::Create(geoFramebufferSpec);
		m_GeoPass = RenderPass::Create(geoRenderPassSpec);

		FramebufferSpecification compFramebufferSpec;
		compFramebufferSpec.Width = 1280;
		compFramebufferSpec.Height = 720;
		compFramebufferSpec.Format = FramebufferFormat::RGBA8;
		compFramebufferSpec.ClearColor = { 0.1f, 0.1f, 0.1f, 1.0f };

		RenderPassSpecification compRenderPassSpec;
		compRenderPassSpec.TargetFramebuffer = Flush::Framebuffer::Create(compFramebufferSpec);
		m_CompositePass = RenderPass::Create(compRenderPassSpec);
#pragma endregion

		float x = -4.0f;
		float roughness = 0.0f;
		for (int i = 0; i < 8; i++)
		{
			Ref<MaterialInstance> mi(new MaterialInstance(m_SphereMesh->GetMaterial()));
			mi->Set("u_Metalness", 1.0f);
			mi->Set("u_Roughness", roughness);
			mi->Set("u_ModelMatrix", translate(mat4(1.0f), vec3(x, 0.0f, 0.0f)));
			x += 1.1f;
			roughness += 0.15f;
			m_MetalSphereMaterialInstances.push_back(mi);
		}

		x = -4.0f;
		roughness = 0.0f;
		for (int i = 0; i < 8; i++)
		{
			Ref<MaterialInstance> mi(new MaterialInstance(m_SphereMesh->GetMaterial()));
			mi->Set("u_Metalness", 0.0f);
			mi->Set("u_Roughness", roughness);
			mi->Set("u_ModelMatrix", translate(mat4(1.0f), vec3(x, 1.2f, 0.0f)));
			x += 1.1f;
			roughness += 0.15f;
			m_DielectricSphereMaterialInstances.push_back(mi);
		}

		// Create fullscreen quad for final composite
		x = -1;
		float y = -1;
		float width = 2, height = 2;
		struct QuadVertex
		{
			glm::vec3 Position;
			glm::vec2 TexCoord;
		};

		QuadVertex* data = new QuadVertex[4];

		data[0].Position = glm::vec3(-1, -1, 0);
		data[0].TexCoord = glm::vec2(0, 0);

		data[1].Position = glm::vec3(1, -1, 0);
		data[1].TexCoord = glm::vec2(1, 0);

		data[2].Position = glm::vec3(1, 1, 0);
		data[2].TexCoord = glm::vec2(1, 1);

		data[3].Position = glm::vec3(-1, 1, 0);
		data[3].TexCoord = glm::vec2(0, 1);

		m_FullscreenQuadVertexArray = VertexArray::Create();
		auto quadVB = VertexBuffer::Create(data, 4 * sizeof(QuadVertex));
		quadVB->SetLayout({
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float2, "a_TexCoord" }
			});

		uint32_t indices[6] = { 0, 1, 2, 2, 3, 0, };
		auto quadIB = IndexBuffer::Create(indices, 6 * sizeof(uint32_t));

		m_FullscreenQuadVertexArray->AddVertexBuffer(quadVB);
		m_FullscreenQuadVertexArray->SetIndexBuffer(quadIB);

		// Set lights
		m_Light.Direction = { -0.5f, -0.5f, 1.0f };
		m_Light.Radiance = { 1.0f, 1.0f, 1.0f };

		m_Transform = glm::scale(glm::mat4(1.0f), glm::vec3(m_MeshScale));
	}

	void PBREditorLayer::OnDetach()
	{
	}

	void PBREditorLayer::OnUpdate(Timestep ts)
	{
		// THINGS TO LOOK AT:
		// - BRDF LUT
		// - Cubemap mips and filtering
		// - Tonemapping and proper HDR pipeline
		using namespace Flush;
		using namespace glm;

		m_Camera.Update(ts);
		auto viewProjection = m_Camera.GetProjectionMatrix() * m_Camera.GetViewMatrix();

		m_Mesh->OnUpdate(ts);

#pragma region Geo RenderBuffer
		Renderer::BeginRenderPass(m_GeoPass);
		// TODO:
		// Renderer::BeginScene(m_Camera);
		// Renderer::EndScene();

		m_QuadShader->Bind();
		m_QuadShader->SetMat4("u_InverseVP", inverse(viewProjection));
		// TODO: Added skybox
		{
			m_EnvironmentCubeMap->Bind(0);
			m_FullscreenQuadVertexArray->Bind();
			Renderer::DrawIndexed(m_FullscreenQuadVertexArray->GetIndexBuffer()->GetCount(), false);
		}

#pragma region 更新材质参数
		m_MeshMaterial->Set("u_AlbedoColor", m_AlbedoInput.Color);
		m_MeshMaterial->Set("u_Metalness", m_MetalnessInput.Value);
		m_MeshMaterial->Set("u_Roughness", m_RoughnessInput.Value);
		m_MeshMaterial->Set("u_ViewProjectionMatrix", viewProjection);
		m_MeshMaterial->Set("u_ModelMatrix", scale(mat4(1.0f), vec3(m_MeshScale)));
		m_MeshMaterial->Set("lights", m_Light);
		m_MeshMaterial->Set("u_CameraPosition", m_Camera.GetPosition());
		m_MeshMaterial->Set("u_RadiancePrefilter", m_RadiancePrefilter ? 1.0f : 0.0f);
		m_MeshMaterial->Set("u_AlbedoTexToggle", m_AlbedoInput.UseTexture ? 1.0f : 0.0f);
		m_MeshMaterial->Set("u_NormalTexToggle", m_NormalInput.UseTexture ? 1.0f : 0.0f);
		m_MeshMaterial->Set("u_MetalnessTexToggle", m_MetalnessInput.UseTexture ? 1.0f : 0.0f);
		m_MeshMaterial->Set("u_RoughnessTexToggle", m_RoughnessInput.UseTexture ? 1.0f : 0.0f);
		m_MeshMaterial->Set("u_EnvMapRotation", m_EnvMapRotation);

		m_MeshMaterial->Set("u_EnvRadianceTex", m_EnvironmentCubeMap);
		m_MeshMaterial->Set("u_EnvIrradianceTex", m_EnvironmentIrradiance);
		m_MeshMaterial->Set("u_BRDFLUTTexture", m_BRDFLUT);

		m_SphereMesh->GetMaterial()->Set("u_AlbedoColor", m_AlbedoInput.Color);
		m_SphereMesh->GetMaterial()->Set("u_Metalness", m_MetalnessInput.Value);
		m_SphereMesh->GetMaterial()->Set("u_Roughness", m_RoughnessInput.Value);
		m_SphereMesh->GetMaterial()->Set("u_ViewProjectionMatrix", viewProjection);
		m_SphereMesh->GetMaterial()->Set("u_ModelMatrix", scale(mat4(1.0f), vec3(m_MeshScale)));
		m_SphereMesh->GetMaterial()->Set("lights", m_Light);
		m_SphereMesh->GetMaterial()->Set("u_CameraPosition", m_Camera.GetPosition());
		m_SphereMesh->GetMaterial()->Set("u_RadiancePrefilter", m_RadiancePrefilter ? 1.0f : 0.0f);
		m_SphereMesh->GetMaterial()->Set("u_AlbedoTexToggle", m_AlbedoInput.UseTexture ? 1.0f : 0.0f);
		m_SphereMesh->GetMaterial()->Set("u_NormalTexToggle", m_NormalInput.UseTexture ? 1.0f : 0.0f);
		m_SphereMesh->GetMaterial()->Set("u_MetalnessTexToggle", m_MetalnessInput.UseTexture ? 1.0f : 0.0f);
		m_SphereMesh->GetMaterial()->Set("u_RoughnessTexToggle", m_RoughnessInput.UseTexture ? 1.0f : 0.0f);
		m_SphereMesh->GetMaterial()->Set("u_EnvMapRotation", m_EnvMapRotation);
		m_SphereMesh->GetMaterial()->Set("u_EnvRadianceTex", m_EnvironmentCubeMap);
		m_SphereMesh->GetMaterial()->Set("u_EnvIrradianceTex", m_EnvironmentIrradiance);
		m_SphereMesh->GetMaterial()->Set("u_BRDFLUTTexture", m_BRDFLUT);

		if (m_AlbedoInput.TextureMap)
			m_MeshMaterial->Set("u_AlbedoTexture", m_AlbedoInput.TextureMap);
		if (m_NormalInput.TextureMap)
			m_MeshMaterial->Set("u_NormalTexture", m_NormalInput.TextureMap);
		if (m_MetalnessInput.TextureMap)
			m_MeshMaterial->Set("u_MetalnessTexture", m_MetalnessInput.TextureMap);
		if (m_RoughnessInput.TextureMap)
			m_MeshMaterial->Set("u_RoughnessTexture", m_RoughnessInput.TextureMap);
#pragma endregion

		if (m_Scene == Scene::Spheres)
		{
			// Metals
			for (int i = 0; i < 8; i++)
				Renderer::SubmitMesh(m_SphereMesh, glm::mat4(1.0f), m_MetalSphereMaterialInstances[i]);

			// Dielectrics
			for (int i = 0; i < 8; i++)
				Renderer::SubmitMesh(m_SphereMesh, glm::mat4(1.0f), m_DielectricSphereMaterialInstances[i]);
		}
		else if (m_Scene == Scene::Model)
		{
			if (m_Mesh)
				Renderer::SubmitMesh(m_Mesh, m_Transform, m_MeshMaterial);
		}

		m_GridMaterial->Set("u_MVP", viewProjection * glm::scale(glm::mat4(1.0f), glm::vec3(16.0f)));
		Renderer::SubmitMesh(m_PlaneMesh, glm::mat4(1.0f), m_GridMaterial);

		Renderer::EndRenderPass();
#pragma endregion

		Renderer::BeginRenderPass(m_CompositePass);
		m_HDRShader->Bind();
		m_HDRShader->SetFloat("u_Exposure", m_Exposure);
		m_GeoPass->GetSpecification().TargetFramebuffer->BindTexture();
		m_FullscreenQuadVertexArray->Bind();
		Renderer::DrawIndexed(m_FullscreenQuadVertexArray->GetIndexBuffer()->GetCount(), false);
		Renderer::EndRenderPass();
	}

#pragma region GUI Properties

	void PBREditorLayer::Property(const std::string& name, bool& value)
	{
		ImGui::Text(name.c_str());
		ImGui::NextColumn();
		ImGui::PushItemWidth(-1);

		std::string id = "##" + name;
		ImGui::Checkbox(id.c_str(), &value);

		ImGui::PopItemWidth();
		ImGui::NextColumn();
	}

	void PBREditorLayer::Property(const std::string& name, float& value, float min, float max, PBREditorLayer::PropertyFlag flags)
	{
		ImGui::Text(name.c_str());
		ImGui::NextColumn();
		ImGui::PushItemWidth(-1);

		std::string id = "##" + name;
		ImGui::SliderFloat(id.c_str(), &value, min, max);

		ImGui::PopItemWidth();
		ImGui::NextColumn();
	}

	void PBREditorLayer::Property(const std::string& name, glm::vec3& value, PBREditorLayer::PropertyFlag flags)
	{
		Property(name, value, -1.0f, 1.0f, flags);
	}

	void PBREditorLayer::Property(const std::string& name, glm::vec3& value, float min, float max, PBREditorLayer::PropertyFlag flags)
	{
		ImGui::Text(name.c_str());
		ImGui::NextColumn();
		ImGui::PushItemWidth(-1);

		std::string id = "##" + name;
		if ((int)flags & (int)PropertyFlag::ColorProperty)
			ImGui::ColorEdit3(id.c_str(), glm::value_ptr(value), ImGuiColorEditFlags_NoInputs);
		else
			ImGui::SliderFloat3(id.c_str(), glm::value_ptr(value), min, max);

		ImGui::PopItemWidth();
		ImGui::NextColumn();
	}

	void PBREditorLayer::Property(const std::string& name, glm::vec4& value, PBREditorLayer::PropertyFlag flags)
	{
		Property(name, value, -1.0f, 1.0f, flags);
	}

	void PBREditorLayer::Property(const std::string& name, glm::vec4& value, float min, float max, PBREditorLayer::PropertyFlag flags)
	{
		ImGui::Text(name.c_str());
		ImGui::NextColumn();
		ImGui::PushItemWidth(-1);

		std::string id = "##" + name;
		if ((int)flags & (int)PropertyFlag::ColorProperty)
			ImGui::ColorEdit4(id.c_str(), glm::value_ptr(value), ImGuiColorEditFlags_NoInputs);
		else
			ImGui::SliderFloat4(id.c_str(), glm::value_ptr(value), min, max);

		ImGui::PopItemWidth();
		ImGui::NextColumn();
	}
#pragma endregion
	void PBREditorLayer::OnImGuiRender()
	{
		static bool p_open = true;

		static bool opt_fullscreen_persistant = true;
		static ImGuiDockNodeFlags opt_flags = ImGuiDockNodeFlags_None;
		bool opt_fullscreen = opt_fullscreen_persistant;

		// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
		// because it would be confusing to have two docking targets within each others.
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		if (opt_fullscreen)
		{
			ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->Pos);
			ImGui::SetNextWindowSize(viewport->Size);
			ImGui::SetNextWindowViewport(viewport->ID);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
			window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		}

		// When using ImGuiDockNodeFlags_PassthruDockspace, DockSpace() will render our background and handle the pass-thru hole, so we ask Begin() to not render a background.
		//if (opt_flags & ImGuiDockNodeFlags_PassthruDockspace)
		//	window_flags |= ImGuiWindowFlags_NoBackground;

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("DockSpace Demo", &p_open, window_flags);
		ImGui::PopStyleVar();

		if (opt_fullscreen)
			ImGui::PopStyleVar(2);

		// Dockspace
		ImGuiIO& io = ImGui::GetIO();
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID("MyDockspace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), opt_flags);
		}

		// Editor Panel ------------------------------------------------------------------------------
		ImGui::Begin("Model");
		ImGui::RadioButton("Spheres", (int*)& m_Scene, (int)Scene::Spheres);
		ImGui::SameLine();
		ImGui::RadioButton("Model", (int*)& m_Scene, (int)Scene::Model);

		ImGui::Begin("Environment");

		ImGui::Columns(2);
		ImGui::AlignTextToFramePadding();

		Property("Light Direction", m_Light.Direction);
		Property("Light Radiance", m_Light.Radiance, PropertyFlag::ColorProperty);
		Property("Light Multiplier", m_LightMultiplier, 0.0f, 5.0f);
		Property("Exposure", m_Exposure, 0.0f, 5.0f);

		Property("Mesh Scale", m_MeshScale, 0.0f, 2.0f);

		Property("Radiance Prefiltering", m_RadiancePrefilter);
		Property("Env Map Rotation", m_EnvMapRotation, -360.0f, 360.0f);

		ImGui::Columns(1);

		ImGui::End();

		ImGui::Separator();
		{
			ImGui::Text("Mesh");
			std::string fullpath = m_Mesh ? m_Mesh->GetFilePath() : "None";
			size_t found = fullpath.find_last_of("/\\");
			std::string path = found != std::string::npos ? fullpath.substr(found + 1) : fullpath;
			ImGui::Text(path.c_str()); ImGui::SameLine();
			if (ImGui::Button("...##Mesh"))
			{
				std::string filename = Application::Get().OpenFile("");
				if (filename != "")
				{
					m_Mesh.reset(new Mesh(filename));
					m_MeshMaterial.reset(new MaterialInstance(m_Mesh->GetMaterial()));
				}
			}
		}
		ImGui::Separator();

		// Textures ------------------------------------------------------------------------------
		{
			// Albedo
			if (ImGui::CollapsingHeader("Albedo", nullptr, ImGuiTreeNodeFlags_DefaultOpen))
			{
				ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(10, 10));
				ImGui::Image(m_AlbedoInput.TextureMap ? (void*)m_AlbedoInput.TextureMap->GetRendererID() : (void*)m_CheckerboardTex->GetRendererID(), ImVec2(64, 64));
				ImGui::PopStyleVar();
				if (ImGui::IsItemHovered())
				{
					if (m_AlbedoInput.TextureMap)
					{
						ImGui::BeginTooltip();
						ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
						ImGui::TextUnformatted(m_AlbedoInput.TextureMap->GetPath().c_str());
						ImGui::PopTextWrapPos();
						ImGui::Image((void*)m_AlbedoInput.TextureMap->GetRendererID(), ImVec2(384, 384));
						ImGui::EndTooltip();
					}
					if (ImGui::IsItemClicked())
					{
						std::string filename = Application::Get().OpenFile("");
						if (filename != "")
							m_AlbedoInput.TextureMap.reset(Texture2D::Create(filename, m_AlbedoInput.SRGB));
					}
				}
				ImGui::SameLine();
				ImGui::BeginGroup();
				ImGui::Checkbox("Use##AlbedoMap", &m_AlbedoInput.UseTexture);
				if (ImGui::Checkbox("sRGB##AlbedoMap", &m_AlbedoInput.SRGB))
				{
					if (m_AlbedoInput.TextureMap)
						m_AlbedoInput.TextureMap.reset(Texture2D::Create(m_AlbedoInput.TextureMap->GetPath(), m_AlbedoInput.SRGB));
				}
				ImGui::EndGroup();
				ImGui::SameLine();
				ImGui::ColorEdit3("Color##Albedo", glm::value_ptr(m_AlbedoInput.Color), ImGuiColorEditFlags_NoInputs);
			}
		}
		{
			// Normals
			if (ImGui::CollapsingHeader("Normals", nullptr, ImGuiTreeNodeFlags_DefaultOpen))
			{
				ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(10, 10));
				ImGui::Image(m_NormalInput.TextureMap ? (void*)m_NormalInput.TextureMap->GetRendererID() : (void*)m_CheckerboardTex->GetRendererID(), ImVec2(64, 64));
				ImGui::PopStyleVar();
				if (ImGui::IsItemHovered())
				{
					if (m_NormalInput.TextureMap)
					{
						ImGui::BeginTooltip();
						ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
						ImGui::TextUnformatted(m_NormalInput.TextureMap->GetPath().c_str());
						ImGui::PopTextWrapPos();
						ImGui::Image((void*)m_NormalInput.TextureMap->GetRendererID(), ImVec2(384, 384));
						ImGui::EndTooltip();
					}
					if (ImGui::IsItemClicked())
					{
						std::string filename = Application::Get().OpenFile("");
						if (filename != "")
							m_NormalInput.TextureMap.reset(Texture2D::Create(filename));
					}
				}
				ImGui::SameLine();
				ImGui::Checkbox("Use##NormalMap", &m_NormalInput.UseTexture);
			}
		}
		{
			// Metalness
			if (ImGui::CollapsingHeader("Metalness", nullptr, ImGuiTreeNodeFlags_DefaultOpen))
			{
				ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(10, 10));
				ImGui::Image(m_MetalnessInput.TextureMap ? (void*)m_MetalnessInput.TextureMap->GetRendererID() : (void*)m_CheckerboardTex->GetRendererID(), ImVec2(64, 64));
				ImGui::PopStyleVar();
				if (ImGui::IsItemHovered())
				{
					if (m_MetalnessInput.TextureMap)
					{
						ImGui::BeginTooltip();
						ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
						ImGui::TextUnformatted(m_MetalnessInput.TextureMap->GetPath().c_str());
						ImGui::PopTextWrapPos();
						ImGui::Image((void*)m_MetalnessInput.TextureMap->GetRendererID(), ImVec2(384, 384));
						ImGui::EndTooltip();
					}
					if (ImGui::IsItemClicked())
					{
						std::string filename = Application::Get().OpenFile("");
						if (filename != "")
							m_MetalnessInput.TextureMap.reset(Texture2D::Create(filename));
					}
				}
				ImGui::SameLine();
				ImGui::Checkbox("Use##MetalnessMap", &m_MetalnessInput.UseTexture);
				ImGui::SameLine();
				ImGui::SliderFloat("Value##MetalnessInput", &m_MetalnessInput.Value, 0.0f, 1.0f);
			}
		}
		{
			// Roughness
			if (ImGui::CollapsingHeader("Roughness", nullptr, ImGuiTreeNodeFlags_DefaultOpen))
			{
				ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(10, 10));
				ImGui::Image(m_RoughnessInput.TextureMap ? (void*)m_RoughnessInput.TextureMap->GetRendererID() : (void*)m_CheckerboardTex->GetRendererID(), ImVec2(64, 64));
				ImGui::PopStyleVar();
				if (ImGui::IsItemHovered())
				{
					if (m_RoughnessInput.TextureMap)
					{
						ImGui::BeginTooltip();
						ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
						ImGui::TextUnformatted(m_RoughnessInput.TextureMap->GetPath().c_str());
						ImGui::PopTextWrapPos();
						ImGui::Image((void*)m_RoughnessInput.TextureMap->GetRendererID(), ImVec2(384, 384));
						ImGui::EndTooltip();
					}
					if (ImGui::IsItemClicked())
					{
						std::string filename = Application::Get().OpenFile("");
						if (filename != "")
							m_RoughnessInput.TextureMap.reset(Texture2D::Create(filename));
					}
				}
				ImGui::SameLine();
				ImGui::Checkbox("Use##RoughnessMap", &m_RoughnessInput.UseTexture);
				ImGui::SameLine();
				ImGui::SliderFloat("Value##RoughnessInput", &m_RoughnessInput.Value, 0.0f, 1.0f);
			}
		}

		ImGui::Separator();

		if (ImGui::TreeNode("Shaders"))
		{
			auto& shaders = Shader::s_AllShaders;
			for (auto& shader : shaders)
			{
				if (ImGui::TreeNode(shader->GetName().c_str()))
				{
					std::string buttonName = "Reload##" + shader->GetName();
					if (ImGui::Button(buttonName.c_str()))
						shader->Reload();
					ImGui::TreePop();
				}
			}
			ImGui::TreePop();
		}


		ImGui::End();

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
		ImGui::Begin("Viewport");

		auto viewportSize = ImGui::GetContentRegionAvail();

		m_GeoPass->GetSpecification().TargetFramebuffer->Resize((uint32_t)viewportSize.x, (uint32_t)viewportSize.y);
		m_CompositePass->GetSpecification().TargetFramebuffer->Resize((uint32_t)viewportSize.x, (uint32_t)viewportSize.y);
		m_Camera.SetProjectionMatrix(glm::perspectiveFov(glm::radians(45.0f), viewportSize.x, viewportSize.y, 0.1f, 10000.0f));
		m_Camera.SetViewportSize((uint32_t)viewportSize.x, (uint32_t)viewportSize.y);
		ImGui::Image((void*)m_CompositePass->GetSpecification().TargetFramebuffer->GetColorAttachmentRendererID(), viewportSize, { 0, 1 }, { 1, 0 });

		// Gizmos
		if (m_GizmoType != -1)
		{
			float rw = (float)ImGui::GetWindowWidth();
			float rh = (float)ImGui::GetWindowHeight();
			ImGuizmo::SetOrthographic(false);
			ImGuizmo::SetDrawlist();
			ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, rw, rh);
			ImGuizmo::Manipulate(glm::value_ptr(m_Camera.GetViewMatrix()), glm::value_ptr(m_Camera.GetProjectionMatrix()), (ImGuizmo::OPERATION)m_GizmoType, ImGuizmo::LOCAL, glm::value_ptr(m_Transform));
		}

		ImGui::End();
		ImGui::PopStyleVar();

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("Docking"))
			{
				// Disabling fullscreen would allow the window to be moved to the front of other windows, 
				// which we can't undo at the moment without finer window depth/z control.
				//ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen_persistant);

				if (ImGui::MenuItem("Flag: NoSplit", "", (opt_flags & ImGuiDockNodeFlags_NoSplit) != 0))                 opt_flags ^= ImGuiDockNodeFlags_NoSplit;
				if (ImGui::MenuItem("Flag: NoDockingInCentralNode", "", (opt_flags & ImGuiDockNodeFlags_NoDockingInCentralNode) != 0))  opt_flags ^= ImGuiDockNodeFlags_NoDockingInCentralNode;
				if (ImGui::MenuItem("Flag: NoResize", "", (opt_flags & ImGuiDockNodeFlags_NoResize) != 0))                opt_flags ^= ImGuiDockNodeFlags_NoResize;
				//if (ImGui::MenuItem("Flag: PassthruDockspace", "", (opt_flags & ImGuiDockNodeFlags_PassthruDockspace) != 0))       opt_flags ^= ImGuiDockNodeFlags_PassthruDockspace;
				if (ImGui::MenuItem("Flag: AutoHideTabBar", "", (opt_flags & ImGuiDockNodeFlags_AutoHideTabBar) != 0))          opt_flags ^= ImGuiDockNodeFlags_AutoHideTabBar;
				ImGui::Separator();
				if (ImGui::MenuItem("Close DockSpace", NULL, false, p_open != NULL))
					p_open = false;
				ImGui::EndMenu();
			}
			ImGuiShowHelpMarker(
				"You can _always_ dock _any_ window into another by holding the SHIFT key while moving a window. Try it now!" "\n"
				"This demo app has nothing to do with it!" "\n\n"
				"This demo app only demonstrate the use of ImGui::DockSpace() which allows you to manually create a docking node _within_ another window. This is useful so you can decorate your main application window (e.g. with a menu bar)." "\n\n"
				"ImGui::DockSpace() comes with one hard constraint: it needs to be submitted _before_ any window which may be docked into it. Therefore, if you use a dock spot as the central point of your application, you'll probably want it to be part of the very first window you are submitting to imgui every frame." "\n\n"
				"(NB: because of this constraint, the implicit \"Debug\" window can not be docked into an explicit DockSpace() node, because that window is submitted as part of the NewFrame() call. An easy workaround is that you can create your own implicit \"Debug##2\" window after calling DockSpace() and leave it in the window stack for anyone to use.)"
			);

			ImGui::EndMenuBar();
		}

		ImGui::End();

		if (m_Mesh)
			m_Mesh->OnImGuiRender();
	}

	void PBREditorLayer::OnEvent(Event& event)
	{
		EventDispatcher dispatcher(event);
		// #define F_BIND_EVENT_FN(fn) std::bind(&##fn, this, std::placeholders::_1)
		dispatcher.Dispatch<KeyPressedEvent>(F_BIND_EVENT_FN(PBREditorLayer::OnKeyPressedEvent));
	}


	bool PBREditorLayer::OnKeyPressedEvent(KeyPressedEvent& e)
	{
		switch (e.GetKeyCode())
		{
		case F_KEY_Q:
			m_GizmoType = -1;
			break;
		case F_KEY_W:
			m_GizmoType = ImGuizmo::OPERATION::TRANSLATE;
			break;
		case F_KEY_E:
			m_GizmoType = ImGuizmo::OPERATION::ROTATE;
			break;
		case F_KEY_R:
			m_GizmoType = ImGuizmo::OPERATION::SCALE;
			break;
		}
		return false;
	}

}