//
//  graphics_mesh_interface.c
//  graphics
//
//  Created by Евгений Лютц on 23.01.21.
//

#include "private_interface.h"

GRAPHICS_MESH* graphicsDeviceCreateMesh(GRAPHICS_DEVICE* device, GRAPHICS_VERTEX* vertices, unsigned int numVertices, TEXTURE2D* texture)
{
	assert(device);
	assert(vertices);
	assert(numVertices > 0);
	assert(texture);
	
	GRAPHICS_MESH* mesh = arrayAddItem(&device->meshes);
	mesh->device = device;
	mesh->numVertices = numVertices;
	mesh->meshId = device->createMeshFunc(device, vertices, numVertices, texture);
	
	return mesh;
}

void graphicsMeshRelease(GRAPHICS_MESH* mesh)
{
	assert(mesh);
	
	GRAPHICS_DEVICE* graphicsDevice = mesh->device;
	graphicsDevice->releaseMeshFunc(mesh);
	
	arrayRemoveItem(&graphicsDevice->meshes, mesh);
}


GRAPHICS_MESH_UNIFORMS* graphicsDeviceCreateMeshUniforms(GRAPHICS_DEVICE* device)
{
	assert(device);
	
	GRAPHICS_MESH_UNIFORMS* uniforms = arrayAddItem(&device->meshUniforms);
	uniforms->device = device;
	uniforms->uniformsId = device->createMeshUniformsFunc(device);
	
	return uniforms;
}

void graphicsMeshUniformsRelease(GRAPHICS_MESH_UNIFORMS* uniforms)
{
	assert(uniforms);
	
	GRAPHICS_DEVICE* device = uniforms->device;
	device->meshUniformsReleaseFunc(uniforms);
	arrayRemoveItem(&device->meshUniforms, uniforms);
}

void graphicsMeshUniformsSetData(GRAPHICS_MESH_UNIFORMS* uniforms, COMMAND_QUEUE* queue, GRAPHICS_MESH_UNIFORMS_DATA* data)
{
	assert(uniforms);
	assert(queue);
	assert(data);
	
	uniforms->device->meshUniformsSetDataFunc(uniforms, queue, data);
}
