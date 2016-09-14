// 
//---------------------------------------------------------------------------
//
// Copyright(C) 2016 Magnus Norddahl
// All rights reserved.
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/
//
//--------------------------------------------------------------------------
//
/*
** gl_tonemapshader.cpp
** Converts a HDR texture to 0-1 range by applying a tonemap operator
**
*/

#include "gl/system/gl_system.h"
#include "files.h"
#include "m_swap.h"
#include "v_video.h"
#include "gl/gl_functions.h"
#include "vectors.h"
#include "gl/system/gl_interface.h"
#include "gl/system/gl_framebuffer.h"
#include "gl/system/gl_cvars.h"
#include "gl/shaders/gl_tonemapshader.h"

void FTonemapShader::Bind()
{
	auto &shader = mShader[gl_tonemap];
	if (!shader)
	{
		shader.Compile(FShaderProgram::Vertex, "shaders/glsl/screenquad.vp", "", 330);
		shader.Compile(FShaderProgram::Fragment, "shaders/glsl/tonemap.fp", GetDefines(gl_tonemap), 330);
		shader.SetFragDataLocation(0, "FragColor");
		shader.Link("shaders/glsl/tonemap");
		shader.SetAttribLocation(0, "PositionInProjection");
		SceneTexture.Init(shader, "InputTexture");
		Exposure.Init(shader, "ExposureAdjustment");
		PaletteLUT.Init(shader, "PaletteLUT");
	}
	shader.Bind();
}

bool FTonemapShader::IsPaletteMode()
{
	return gl_tonemap == Palette;
}

const char *FTonemapShader::GetDefines(int mode)
{
	switch (mode)
	{
	default:
	case Linear:     return "#define LINEAR\n";
	case Reinhard:   return "#define REINHARD\n";
	case HejlDawson: return "#define HEJLDAWSON\n";
	case Uncharted2: return "#define UNCHARTED2\n";
	case Palette:    return "#define PALETTE\n";
	}
}
