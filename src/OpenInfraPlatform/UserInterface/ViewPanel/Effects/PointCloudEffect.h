/*
Copyright (c) 2018 Technical University of Munich
Chair of Computational Modeling and Simulation.

TUM Open Infra Platform is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License Version 3
as published by the Free Software Foundation.

TUM Open Infra Platform is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program. If not, see <http://www.gnu.org/licenses/>.
*/


#pragma once
#ifndef OpenInfraPlatform_UserInterface_PointCloudEffect_C9932E98_6B4E_4E36_8B8D_5AA32D41AC0F_h
#define OpenInfraPlatform_UserInterface_PointCloudEffect_C9932E98_6B4E_4E36_8B8D_5AA32D41AC0F_h

#include "OpenInfraPlatform/namespace.h"
#include "OpenInfraPlatform/Infrastructure/PointCloudProcessing/PointCloudProcessing.h"

#include <buw.Rasterizer.h>
#include <map>

OIP_NAMESPACE_OPENINFRAPLATFORM_UI_BEGIN

class PointCloudEffect : public buw::Effect {
public:

	/*Struct for possible settings*/
	struct SettingsBuffer {
		BlueFramework::Rasterizer::AlignedTo16Byte::Float4 positions[4];
		BlueFramework::Rasterizer::AlignedTo16Byte::Float pointSize = 3.0f;
		BlueFramework::Rasterizer::AlignedTo16Byte::Int bUseUniformPointSize = false;
		BlueFramework::Rasterizer::AlignedTo16Byte::Int bUseUniformColor = false;
	};

	/*Construct by providing the renderSystem, viewport, depthStencil and worldBuffer for camera etc.*/
	PointCloudEffect(buw::IRenderSystem* renderSystem,
		buw::ReferenceCounted<buw::IViewport> viewport,
		buw::ReferenceCounted<buw::ITexture2D> depthStencilMSAA,
		buw::ReferenceCounted<buw::IConstantBuffer> worldBuffer,
		buw::ReferenceCounted<buw::IConstantBuffer> viewportBuffer);

	/*Release all resources*/
	virtual ~PointCloudEffect();

	/*Load the shader from effect file and construct pipelineState*/
	void loadShader();

	/*Turn colored drawing on or off*/
	void drawPointsWithUniformColor(const bool checked);

	void drawPointsWithUniformSize(const bool checked);

	/*Set the point size for the geometry shader*/
	void setPointSize(const float size);

	/*Set the point cloud to be rendered*/
	void setPointCloud(buw::ReferenceCounted<OpenInfraPlatform::Infrastructure::PointCloud> pointCloud, buw::Vector3d offset);


private:
	void v_init();
	void v_render();

	void updateSettingsBuffer();

private:
	buw::ReferenceCounted<buw::IConstantBuffer> worldBuffer_ = nullptr;
	buw::ReferenceCounted<buw::IConstantBuffer> settingsBuffer_ = nullptr;
	buw::ReferenceCounted<buw::IConstantBuffer> viewportBuffer_ = nullptr;
	buw::ReferenceCounted<buw::IVertexBuffer> vertexBuffer_ = nullptr;
	buw::ReferenceCounted<buw::ITexture2D> depthStencilMSAA_ = nullptr;
	buw::ReferenceCounted<buw::IPipelineState> pipelineState_ = nullptr;
	buw::ReferenceCounted<buw::IViewport> viewport_ = nullptr;
	SettingsBuffer settings_;
};

OIP_NAMESPACE_OPENINFRAPLATFORM_UI_END

#endif //end define OpenInfraPlatform_UserInterface_PointCloudEffect_C9932E98_6B4E_4E36_8B8D_5AA32D41AC0F_h