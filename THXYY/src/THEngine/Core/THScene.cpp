#include "THScene.h"

using namespace THEngine;

void Scene::AddLayer(Layer* layer)
{
	auto iter = layers.GetIterator();
	while (iter->HasNext())
	{
		Layer* curLayer = iter->Next();
		if (curLayer->order < layer->order)
		{
			iter->AddBefore(layer);
			return;
		}
	}
	iter->AddAfter(layer);
}

void Scene::Update()
{
	auto iter = layers.GetIterator();
	while (iter->HasNext())
	{
		Layer* curLayer = iter->Next();
		curLayer->Update();
	}
}

void Scene::Draw()
{
	auto iter = layers.GetIterator();
	while (iter->HasNext())
	{
		Layer* curLayer = iter->Next();
		curLayer->Draw();
	}
}

void Scene::OnLoad()
{

}

void Scene::OnSceneChanged()
{

}