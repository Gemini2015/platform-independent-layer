#include<Windows.h>
#include<gl\glew.h>
#include<gl\GL.h>
#include<gl\GLU.h>

#include "GLDemo.h"
#include "CustomWindow.h"
#include "PILInclude.h"
#include <conio.h>

const int NumVertexs = 6;
#define BUFFER_OFFSET(offset) ((void*)(offset))

GLDemo::GLDemo()
	: mCustomWindow(nullptr)
{

}

GLDemo::~GLDemo()
{
	if (mCustomWindow)
		delete mCustomWindow;
}

void GLDemo::Setup()
{
	// init window
	if (mCustomWindow)
		return;
	mCustomWindow = new CustomWindow("GLDemo");
	HRESULT hr = mCustomWindow->Create();
	if (FAILED(hr))
	{
		throw std::runtime_error("Create Window Error");
	}
	mCustomWindow->SetHidden(false);

	// reg listener
	PIL::Root::Singleton().GetWindowManger()->AddListener(this);

	// init opengl
	InitGL();
}

void GLDemo::Run()
{
	std::cout << "Demo run. press 'q' to quit.\n" << std::endl;

	PIL::WindowManager* wm = PIL::Root::Singleton().GetWindowManger();
	while (1)
	{
		if (wm) wm->HandleMessage();
		
		if (mCustomWindow)
		{
			Draw();

			mCustomWindow->SwapBuffers();
		}

		if (_kbhit())
		{
			char c = _getche();
			if(c == 'q')
				break;
		}
	}
}

void GLDemo::Shutdown()
{
	// delete vao
	if (glIsVertexArray(mVAO))
		glDeleteVertexArrays(1, &mVAO);

	// release window
	mCustomWindow->ReleaseWindow();

	// unreg listener
	PIL::Root::Singleton().GetWindowManger()->RemoveListener(this);
}

void GLDemo::InitGL()
{
	GLenum error = glewInit();
	if (error != GLEW_OK)
		throw std::runtime_error("glew init error");
	glClearColor(0.0, 0.0, 0.0, 0.0);
	PIL::Size size = mCustomWindow->GetSize();
	glViewport(0, 0, size.width, size.height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//gluPerspective(60, 800 / 600.0, 0.1, 10);
	glOrtho(-1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	InitVAO();
}

void GLDemo::Draw()
{
	glClear(GL_COLOR_BUFFER_BIT);

	glBindVertexArray(mVAO);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);

	glDrawArrays(GL_TRIANGLES, 0, NumVertexs);

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);

	glBindVertexArray(0);
}

void GLDemo::InitVAO()
{
	glGenVertexArrays(1, &mVAO);
	glBindVertexArray(mVAO);

	GLfloat vertexs[NumVertexs][5] = {
			{ -0.9, -0.9, 0.0, 0.0f, 0.5f },
			{ 0.85, -0.9, 0.0, 0.5f, 0.0f },
			{ -0.9, 0.85, 0.5, 0.0f, 0.0f },
			{ 0.9, -0.85, 0.0, 0.0f, 0.5f },
			{ 0.9, 0.9, 0.0, 0.0f, 0.5f },
			{ -0.85, 0.9, 0.0, 0.0f, 0.5f },
	};

	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexs), vertexs, GL_STATIC_DRAW);
	glVertexPointer(2, GL_FLOAT, sizeof(float) * 5, 0);
	glColorPointer(3, GL_FLOAT, 5 * sizeof(float), BUFFER_OFFSET(2 * sizeof(float)));	
}

void GLDemo::OnSetActive(const PIL::Window* w, bool active)
{
}

void GLDemo::OnWindowMove(const PIL::Window* w)
{
}

void GLDemo::OnWindowResize(const PIL::Window* w)
{
	if (w && w->GetUserWindow())
	{
		CustomWindow* customwindow = (CustomWindow*)w->GetUserWindow();
		if (mCustomWindow != customwindow)
			return;

		// update view port
		PIL::Size size = mCustomWindow->GetSize();
		glViewport(0, 0, size.width, size.height);
	}
}
