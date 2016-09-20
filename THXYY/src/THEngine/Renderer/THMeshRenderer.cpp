#include "THMeshRenderer.h"
#include "../Platform/THApplication.h"
#include "../Asset/THAssetManager.h"

namespace THEngine
{
	MeshRenderer::MeshRenderer()
	{

	}

	MeshRenderer::~MeshRenderer()
	{

	}

	MeshRenderer* MeshRenderer::Create()
	{
		MeshRenderer* renderer = new MeshRenderer();
		if (renderer)
		{
			auto assetManager = AssetManager::GetInstance();

			renderer->meshShader = assetManager->CreateShaderFromFile("fx/mesh.fx");
			if (renderer->meshShader)
			{
				renderer->meshShader->SetTechnique("Mesh");
			}
			else
			{
				delete renderer;
				return nullptr;
			}

		}
		return renderer;
	}

	void MeshRenderer::CalcWorldTransform(Mesh* mesh)
	{
		D3DXMATRIX transform;
		D3DXMatrixIdentity(&transform);

		D3DXMATRIX temp;
		D3DXMatrixScaling(&temp, mesh->GetScale().x, mesh->GetScale().y, mesh->GetScale().z);
		transform *= temp;

		D3DXMatrixRotationQuaternion(&temp, &mesh->rotation3D);
		transform *= temp;

		D3DXMatrixTranslation(&temp, floor(0.5f + mesh->position.x), floor(0.5f + mesh->position.y), mesh->position.z);
		transform *= temp;

		Application::GetInstance()->SetWorldTransform(&transform);

	}

	void MeshRenderer::Render(RenderObject* object)
	{
		Mesh* mesh = (Mesh*)object;
		auto app = Application::GetInstance();

		CalcWorldTransform(mesh);

		Material material = mesh->material;

		float argb[4];
		argb[0] = mesh->alpha;
		argb[1] = mesh->color.x;
		argb[2] = mesh->color.y;
		argb[3] = mesh->color.z;

		D3DXMATRIX mv = app->world * app->view;
		D3DXMATRIX normalMatrix;
		D3DXMatrixInverse(&normalMatrix, nullptr, &mv);
		D3DXMatrixTranspose(&normalMatrix, &normalMatrix);

		meshShader->Begin();

		meshShader->SetFloatArray("argb", argb, 4);
		meshShader->SetTexture("tex", material.texture);
		meshShader->SetMatrix("mvMatrix", &mv);
		meshShader->SetMatrix("normalMatrix", &normalMatrix);
		meshShader->SetMatrix("projection", &app->projection);

		meshShader->BeginPass(0);
		if (mesh->mesh)
		{
			mesh->mesh->DrawSubset(0);
		}
		else
		{
			switch (mesh->drawType)
			{
			case Mesh::TRIANGLE_LIST:
				app->GetDevice()->DrawPrimitive(D3DPT_TRIANGLELIST, 0, mesh->vertexCount / 3);
				break;
			case Mesh::TRIANGLE_STRIP:
				app->GetDevice()->SetStreamSource(0, mesh->vertexBuffer, 0, sizeof(MeshVertex));
				app->GetDevice()->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, mesh->vertexCount - 2);
				break;
			}
		}
		meshShader->EndPass();

		meshShader->End();

	}
}