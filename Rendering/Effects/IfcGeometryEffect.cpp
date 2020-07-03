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

#include "IfcGeometryEffect.h"

#include "../UserInterface/ViewPanel/RenderResources.h"
#include <BlueFramework/Rasterizer/vertex.h>

OIP_NAMESPACE_OPENINFRAPLATFORM_UI_BEGIN


IfcGeometryEffect::IfcGeometryEffect(buw::IRenderSystem * renderSystem,
                                     buw::ReferenceCounted<buw::IViewport> viewport,
                                     buw::ReferenceCounted<buw::ITexture2D> depthStencilMSAA,
                                     buw::ReferenceCounted<buw::IConstantBuffer> worldBuffer)
    :
    Effect(renderSystem),
    viewport_(viewport),
    depthStencilMSAA_(depthStencilMSAA),
    worldBuffer_(worldBuffer)
{
}

IfcGeometryEffect::~IfcGeometryEffect() {
    meshPipelineState_ = nullptr;
    polylinePipelineState_ = nullptr;
    meshVertexBuffer_ = nullptr;
    meshIndexBuffer_ = nullptr;
    polylineVertexBuffer_ = nullptr;
    polylineIndexBuffer_ = nullptr;
    worldBuffer_ = nullptr;
    viewport_ = nullptr;
    depthStencilMSAA_ = nullptr;
}

void IfcGeometryEffect::setIfcGeometryModel(buw::ReferenceCounted<Core::IfcGeometryConverter::IfcGeometryModel> ifcGeometryModel, const buw::Vector3d & offset)
{
	ifcGeometryModel_ = ifcGeometryModel;
	offset_ = offset;

    if(!ifcGeometryModel->isEmpty()) {
        buw::vertexBufferDescription vbd;
        buw::indexBufferDescription ibd;
        valid_ = true;        

		if(!ifcGeometryModel->meshDescription_.isEmpty()) {

			std::vector< VertexLayout> vertices;
			vertices.reserve(ifcGeometryModel->meshDescription_.vertices.size());
			for (auto& vertex : ifcGeometryModel->meshDescription_.vertices)
			{
				VertexLayout vtx(vertex);
				vtx.position[0] += offset.x();
				vtx.position[1] += offset.y();
				vtx.position[2] += offset.z();
				vertices.push_back( vtx );
			}

			vbd.data = vertices.data(); // &ifcGeometryModel->meshDescription_.vertices[0];
			vbd.vertexCount = vertices.size(); // ifcGeometryModel->meshDescription_.vertices.size();
            vbd.vertexLayout = buw::VertexPosition3Color3Normal3::getVertexLayout();
            if(meshVertexBuffer_)
                meshVertexBuffer_->uploadData(vbd);
            else
                meshVertexBuffer_ = renderSystem()->createVertexBuffer(vbd);

			BLUE_LOG(trace) << "Done creating IFC geometry meshes vertex buffer. Size:" << QString::number(vertices.size()).toStdString();

            ibd.data = &ifcGeometryModel->meshDescription_.indices[0];
            ibd.indexCount = ifcGeometryModel->meshDescription_.indices.size();
            ibd.format = buw::eIndexBufferFormat::UnsignedInt32;

            if(meshIndexBuffer_)
                meshIndexBuffer_ = nullptr;
            meshIndexBuffer_ = renderSystem()->createIndexBuffer(ibd);
        }
        else {
            meshVertexBuffer_.reset();
            meshIndexBuffer_.reset();
        }

        if(!ifcGeometryModel->polylineDescription_.isEmpty()) {

			std::vector< buw::Vector3f > vertices;
			vertices.reserve(ifcGeometryModel->polylineDescription_.vertices.size());
			for (auto& vertex : ifcGeometryModel->polylineDescription_.vertices)
			{
				buw::Vector3f vtx(vertex);
				vtx.x() += offset.x();
				vertices.push_back(vertex + offset.cast<float>());
			}
            vbd.data = vertices.data();  //&ifcGeometryModel->polylineDescription_.vertices[0];
            vbd.vertexCount = vertices.size(); //ifcGeometryModel->polylineDescription_.vertices.size();
            vbd.vertexLayout = buw::VertexPosition3::getVertexLayout();

            if(polylineVertexBuffer_)
                polylineVertexBuffer_->uploadData(vbd);
            else
                polylineVertexBuffer_ = renderSystem()->createVertexBuffer(vbd);

			BLUE_LOG(trace) << "Done creating IFC geometry polyline vertex buffer. Size:" << QString::number(vertices.size()).toStdString();

            ibd.data = &ifcGeometryModel->polylineDescription_.indices[0];
            ibd.indexCount = ifcGeometryModel->polylineDescription_.indices.size();
			ibd.format = buw::eIndexBufferFormat::UnsignedInt32;

            if(polylineIndexBuffer_)
                polylineIndexBuffer_ = nullptr;
            polylineIndexBuffer_ = renderSystem()->createIndexBuffer(ibd);
        }
        else {
            polylineVertexBuffer_.reset();
            polylineIndexBuffer_.reset();
        }
    }
    else {
        valid_ = false;
    }
}

void IfcGeometryEffect::v_init()
{
    try {
        buw::pipelineStateDescription psd;
		psd.effectFilename = buw::Singleton<RenderResources>::instance().getResourceRootDir() + "/Shader/IfcGeometryEffect.be";
		psd.pipelineStateName = "mesh";
        psd.vertexLayout = buw::VertexPosition3Color3Normal3::getVertexLayout();
        psd.primitiveTopology = buw::ePrimitiveTopology::TriangleList;
        psd.renderTargetFormats = { buw::eTextureFormat::R8G8B8A8_UnsignedNormalizedInt_SRGB };
        psd.useDepth = true;
        psd.useMSAA = true;

        meshPipelineState_ = createPipelineState(psd);

		psd.pipelineStateName = "polyline";
        psd.vertexLayout = buw::VertexPosition3::getVertexLayout();
        psd.primitiveTopology = buw::ePrimitiveTopology::LineList;

        polylinePipelineState_ = createPipelineState(psd);
    }
    catch(...) {
        meshPipelineState_ = nullptr;
        polylinePipelineState_ = nullptr;
        meshVertexBuffer_ = nullptr;
        meshIndexBuffer_ = nullptr;
        polylineVertexBuffer_ = nullptr;
        polylineIndexBuffer_ = nullptr;       
    }
}

void IfcGeometryEffect::v_render()
{
	buw::ReferenceCounted<buw::ITexture2D> renderTarget = renderSystem()->getBackBufferTarget();
	setRenderTarget(renderTarget, depthStencilMSAA_);
	setViewport(viewport_);

    if(meshPipelineState_ && meshVertexBuffer_ && meshIndexBuffer_ && valid_) {
        setPipelineState(meshPipelineState_);
        setConstantBuffer(worldBuffer_, "WorldBuffer");
        setVertexBuffer(meshVertexBuffer_);
        setIndexBuffer(meshIndexBuffer_);
        drawIndexed(static_cast<UINT>(meshIndexBuffer_->getIndexCount()));
    }
    if(polylinePipelineState_ && polylineVertexBuffer_&& polylineIndexBuffer_ && valid_) {
        setPipelineState(polylinePipelineState_);
        setConstantBuffer(worldBuffer_, "WorldBuffer");
        setVertexBuffer(polylineVertexBuffer_);
        setIndexBuffer(polylineIndexBuffer_);
        drawIndexed(static_cast<UINT>(polylineIndexBuffer_->getIndexCount()));
    }
}

OIP_NAMESPACE_OPENINFRAPLATFORM_UI_END