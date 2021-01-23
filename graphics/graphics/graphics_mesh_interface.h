//
//  graphics_mesh_interface.h
//  graphics
//
//  Created by Евгений Лютц on 23.01.21.
//

#ifndef graphics_mesh_interface_h
#define graphics_mesh_interface_h

#include "graphics.h"

GRAPHICS_MESH* graphicsDeviceCreateMesh(GRAPHICS_DEVICE* device, GRAPHICS_VERTEX* vertices, unsigned int numVertices, TEXTURE2D* texture);
void graphicsMeshRelease(GRAPHICS_MESH* mesh);

GRAPHICS_MESH_UNIFORMS* graphicsDeviceCreateMeshUniforms(GRAPHICS_DEVICE* device);
void graphicsMeshUniformsRelease(GRAPHICS_MESH_UNIFORMS* uniforms);
void graphicsMeshUniformsSetData(GRAPHICS_MESH_UNIFORMS* uniforms, COMMAND_QUEUE* queue, GRAPHICS_MESH_UNIFORMS_DATA* data);

#endif /* graphics_mesh_interface_h */
