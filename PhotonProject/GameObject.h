#pragma once

#include "IRunnable.h"
#include "IDrawable.h"
#include "Component.h"
#include "Transform2D.h"
#include "Sprite.h"
#include <string>

class GameObject : public IRunnable, public IDrawable
{
private:
	bool m_active;

	//Container<Component> m_Components;

	Transform2D m_transform;
	Sprite m_sprite;

public:
	std::string name;

	GameObject();
	GameObject(Sprite sprite);
	virtual ~GameObject();

	virtual void SetActive(bool active);
	bool IsActive();

	void SetTransform(Transform2D& transform);
	Transform2D& GetTransform();

	void SetSprite(Sprite sprite);
	Sprite& GetSprite();

	virtual void Start();
	virtual void Update(float deltaTime);
	virtual void Draw();
};