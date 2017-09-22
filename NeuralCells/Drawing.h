#pragma once
#ifndef GL_DRAWING_LIB
#define GL_DRAWING_LIB

#include <GL/glut.h>
#include <string>
#include "Color.h"
#include "math.h"
#define M_PI 3.14159265358979323846
#define M_2PI 6.28318530717958647692

struct Camera
{
	Camera(Vector2D p, float w, float h, float z = 1.0f)
		: pos(p - Vector2D(w, h) / 2)
		, center(p)
		, widht(w)
		, height(h)
		, zoom(z)
	{ }

	Camera(float w, float h, float z = 1.0f)
		: pos(Vector2D(0, 0))
		, widht(w)
		, height(h)
		, zoom(z)
	{ }

	Vector2D pos;
	Vector2D center;

	float widht;
	float height;
	float zoom;
};


static void DrawTriangle(float x1, float y1, float x2, float y2, float x3, float y3, RGBColor color, const Camera& camera)
{
	x1 = (x1 - camera.pos.x) * camera.zoom;
	y1 = (y1 - camera.pos.y) * camera.zoom;
	x2 = (x2 - camera.pos.x) * camera.zoom;
	y2 = (y2 - camera.pos.y) * camera.zoom;
	x3 = (x3 - camera.pos.x) * camera.zoom;
	y3 = (y3 - camera.pos.y) * camera.zoom;

	glColor4f(color.r / 255.0f, color.g / 255.0f, color.b / 255.0f, color.a / 255.0f);
	glBegin(GL_TRIANGLES);
	glVertex2f(x1, y1);
	glVertex2d(x2, y2);
	glVertex2d(x3, y3);
	glEnd();
}


static void DrawFilledCircle(int x, int y, float rad, RGBColor color)
{
	glBegin(GL_TRIANGLE_FAN);
	glColor4f(color.r / 255.0f, color.g / 255.0f, color.b / 255.0f, color.a / 255.0f);

	for (float ang = 0.0f; ang < 360.0f; ang += 10)
	{
		float x1 = static_cast <float> (x)+sin(ang * M_PI / 180) * rad;
		float y1 = static_cast <float> (y)+cos(ang * M_PI / 180) * rad;
		glVertex2f(x1, y1);
	}

	glEnd();
}


static void DrawFilledCircle(int x, int y, float rad, RGBColor color, Camera camera)
{
	glBegin(GL_TRIANGLE_FAN);
	glColor4f(color.r / 255.0f, color.g / 255.0f, color.b / 255.0f, color.a / 255.0f);
	rad *= camera.zoom;
	Vector2D point = (Vector2D(x, y) - camera.pos) * camera.zoom;

	for (float ang = 0.0f; ang < 360.0f; ang += 10)
	{
		float x1 = point.x+sin(ang * M_PI / 180) * rad;
		float y1 = point.y+cos(ang * M_PI / 180) * rad;
		glVertex2f(x1, y1);
	}

	glEnd();
}


static void DrawFilledRect(int x, int y, float w, float h, RGBColor color)
{
	glBegin(GL_QUADS);
	glColor4f(color.r / 255.0f, color.g / 255.0f, color.b / 255.0f, color.a / 255.0f);
	glVertex2f(x, y);
	glVertex2f(x + w, y);
	glVertex2f(x + w, y + h);
	glVertex2f(x, y + h);
	glEnd();
}


static void DrawLineThink(float x1, float y1, float x2, float y2, float thick, RGBColor color)
{
	glLineWidth(thick);
	glColor3f(color.r / 255.0f, color.g / 255.0f, color.b / 255.0f);
	glBegin(GL_LINES);
	glVertex2f(x1, y1);
	glVertex2f(x2, y2);
	glEnd();
}


static void DrawLineThink(float x1, float y1, float x2, float y2, float thick, RGBColor color, const Camera& camera)
{
	x1 = (x1 - camera.pos.x) * camera.zoom;
	y1 = (y1 - camera.pos.y) * camera.zoom;
	x2 = (x2 - camera.pos.x) * camera.zoom;
	y2 = (y2 - camera.pos.y) * camera.zoom;

	glLineWidth(thick);
	glColor3f(color.r / 255.0f, color.g / 255.0f, color.b / 255.0f);
	glBegin(GL_LINES);
	glVertex2f(x1, y1);
	glVertex2f(x2, y2);
	glEnd();
}


static void DrawLineThinkT(float x1, float y1, float x2, float y2, float thick, RGBColor color)
{
	Vector2D side = Vector2D((y2 - y1), -(x2 - x1)).GetNormalize() * thick;

	glColor3f(color.r / 255.0f, color.g / 255.0f, color.b / 255.0f);
	glBegin(GL_QUADS);
	glVertex2f(x1 + side.x, y1 + side.y);
	glVertex2f(x1 - side.x, y1 - side.y);
	glVertex2f(x2 - side.x, y2 - side.y);
	glVertex2f(x2 + side.x, y2 + side.y);
	glEnd();
}


static void DrawLineThinkT(float x1, float y1, float x2, float y2, float thick, RGBColor color, const Camera& camera)
{
	x1 = (x1 - camera.pos.x) * camera.zoom;
	y1 = (y1 - camera.pos.y) * camera.zoom;
	x2 = (x2 - camera.pos.x) * camera.zoom;
	y2 = (y2 - camera.pos.y) * camera.zoom;

	Vector2D side = Vector2D((y2 - y1), -(x2 - x1)).GetNormalize() * thick * camera.zoom;

	glColor3f(color.r / 255.0f, color.g / 255.0f, color.b / 255.0f);
	glBegin(GL_QUADS);
	glVertex2f(x1 + side.x, y1 + side.y);
	glVertex2f(x1 - side.x, y1 - side.y);
	glVertex2f(x2 - side.x, y2 - side.y);
	glVertex2f(x2 + side.x, y2 + side.y);
	glEnd();
}


static void DrawLineThinkT(float x1, float y1, float x2, float y2, float startthick, float endthink, RGBColor color, const Camera& camera)
{
	x1 = (x1 - camera.pos.x) * camera.zoom;
	y1 = (y1 - camera.pos.y) * camera.zoom;
	x2 = (x2 - camera.pos.x) * camera.zoom;
	y2 = (y2 - camera.pos.y) * camera.zoom;

	Vector2D side = Vector2D((y2 - y1), -(x2 - x1)).GetNormalize() * camera.zoom;

	glColor3f(color.r / 255.0f, color.g / 255.0f, color.b / 255.0f);
	glBegin(GL_QUADS);
	glVertex2f(x1 + side.x * startthick, y1 + side.y * startthick);
	glVertex2f(x1 - side.x * startthick, y1 - side.y * startthick);
	glVertex2f(x2 - side.x * endthink, y2 - side.y * endthink);
	glVertex2f(x2 + side.x * endthink, y2 + side.y * endthink);
	glEnd();
}


static void DrawLinesThinkT(const std::vector<Vector2D>& points, float thick, RGBColor color, const Camera& camera)
{
	Vector2D lpoint = points[0];
	Vector2D lside;
	lpoint = (lpoint - camera.pos) * camera.zoom;

	for (int i = 1; i < points.size(); i++)
	{
		Vector2D point = points[i];
		point = (point - camera.pos) * camera.zoom;

		Vector2D side = Vector2D((point.y - lpoint.y), -(point.x - lpoint.x)).GetNormalize() * thick * camera.zoom;

		glColor3f(color.r / 255.0f, color.g / 255.0f, color.b / 255.0f);
		glBegin(GL_QUADS);
		glVertex2f(lpoint.x + side.x, lpoint.y + side.y);
		glVertex2f(lpoint.x - side.x, lpoint.y - side.y);
		glVertex2f(point.x - side.x, point.y - side.y);
		glVertex2f(point.x + side.x, point.y + side.y);
		glEnd();
		lpoint = point;

		if (i > 1)
		{
			glColor3f(color.r / 255.0f, color.g / 255.0f, color.b / 255.0f);
			glBegin(GL_QUADS);
			glVertex2f(lpoint.x + side.x, lpoint.y + side.y);
			glVertex2f(lpoint.x - side.x, lpoint.y - side.y);
			glVertex2f(lpoint.x - lside.x, lpoint.y - lside.y);
			glVertex2f(lpoint.x + lside.x, lpoint.y + lside.y);
			glEnd();
		}
		lside = side;
	}
}


static void DrawLinesThinkT(const std::vector<Vector2D>& points, float startthick, float endthink, RGBColor color, const Camera& camera)
{
	Vector2D lpoint = points[0];
	lpoint = (lpoint - camera.pos) * camera.zoom;
	Vector2D lside;

	for (int i = 1; i < points.size(); i++)
	{
		Vector2D point = points[i];
		point = (point - camera.pos) * camera.zoom;

		Vector2D side = Vector2D((point.y - lpoint.y), -(point.x - lpoint.x)).GetNormalize() * camera.zoom;
		float st = (startthick * (points.size() - i) / points.size());
		float et = (startthick * (points.size() - (i + 1)) / points.size());

		glColor3f(color.r / 255.0f, color.g / 255.0f, color.b / 255.0f);
		glBegin(GL_QUADS);
		glVertex2f(lpoint.x + side.x * st, lpoint.y + side.y * st);
		glVertex2f(lpoint.x - side.x * st, lpoint.y - side.y * st);
		glVertex2f(point.x - side.x * et, point.y - side.y * et);
		glVertex2f(point.x + side.x * et, point.y + side.y * et);
		glEnd();

		if (i > 1)
		{
			glColor3f(color.r / 255.0f, color.g / 255.0f, color.b / 255.0f);
			glBegin(GL_TRIANGLES);
			glVertex2f(lpoint.x, lpoint.y);
			glVertex2f(lpoint.x + side.x * st, lpoint.y + side.y * st);
			glVertex2f(lpoint.x + lside.x, lpoint.y + lside.y);

			glVertex2f(lpoint.x, lpoint.y);
			glVertex2f(lpoint.x - side.x * st, lpoint.y - side.y * st);
			glVertex2f(lpoint.x - lside.x, lpoint.y - lside.y);
			glEnd();
		}
		lpoint = point;
		lside = side * et;
	}
}


static void DrawTextQL(std::string text, int x, int y, float angle, RGBColor color)
{
	glColor4f(color.r / 255.0f, color.g / 255.0f, color.b / 255.0f, color.a / 255.0f);
	glMatrixMode(GL_PROJECTION);
	double *matrix = new double[16];
	glGetDoublev(GL_PROJECTION_MATRIX, matrix);
	glRasterPos2i(x, y);
	for (int i = 0; i < text.size(); i++)
		glutBitmapCharacter(GLUT_BITMAP_9_BY_15, (int)text.c_str()[i]);
	glRotatef(angle, 0.f, 0.f, 1.f);

	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixd(matrix);
	glMatrixMode(GL_MODELVIEW);
	glRotatef(0, 0.f, 0.f, 1.f);
}

#endif /* GL_DRAWING_LIB */