#pragma once

#include <GL/GL.h>
#include "PILWindow.h"

class CustomWindow;

class GLDemo
	: PIL::IWindowEventListener
{
public:
	GLDemo();

	~GLDemo();

	void Setup();

	void Run();

	void Shutdown();

private:
	void InitGL();

	void InitVAO();

	void Draw();

	virtual void OnSetActive(const PIL::Window* w, bool active) override;

	virtual void OnWindowMove(const PIL::Window* w) override;

	virtual void OnWindowResize(const PIL::Window* w) override;

private:
	CustomWindow* mCustomWindow;
	
	GLuint mVAO;
};