#pragma once

class IRunnable
{
public:
	virtual void Start() = 0;
	virtual void Update(float deltaTime) = 0;
};