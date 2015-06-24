/**
 * @file
 * File writing routines
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "writing.h"

/**
 * Find the address of the file's end.
 * @param file A file opened as stream.
 */
unsigned int getPos(FILE *file) {
	unsigned int file_end;
	fseek(file, 0, SEEK_END);
	file_end = ftell(file);
	return file_end;
}

/**
 * Check position after writing and fill with zeros
 * @param file
 * @return the number of zeros written
 */
int align(FILE *file) {
	unsigned int get_pos = getPos(file);
	size_t count = 16 - (get_pos & 0xF);
	char zeros[count];
	memset(zeros, 0, count * sizeof(char));
	fwrite(zeros, sizeof(char), count, file);
	return count;
}

/**
 * Write Views structure from a pointed BCM2 into the specified stream.
 * @param bc_m2_file
 * @param ptr
 * @return
 */
int write_views(FILE *bc_m2_file, BCM2 *ptr) {
	if (ptr->header.nViews > 0) {
		ptr->header.ofsViews = getPos(bc_m2_file);
		int i;
		for (i = 0; i < ptr->header.nViews; i++) {
			fwrite(&ptr->views[i].header, sizeof(ViewsHeader), 1, bc_m2_file);
		}
		align(bc_m2_file);

		for (i = 0; i < ptr->header.nViews; i++) {
			if (ptr->views[i].header.nIndices > 0) {
				ptr->views[i].header.ofsIndices = getPos(bc_m2_file);
				fwrite(ptr->views[i].Indices, sizeof(Vertex),
						ptr->views[i].header.nIndices, bc_m2_file);
				align(bc_m2_file);
			}
			if (ptr->views[i].header.nTriangles > 0) {
				ptr->views[i].header.ofsTriangles = getPos(bc_m2_file);
				fwrite(ptr->views[i].Triangles, sizeof(Triangle),
						ptr->views[i].header.nTriangles / 3, bc_m2_file);
				align(bc_m2_file);
			}
			if (ptr->views[i].header.nProperties > 0) {
				ptr->views[i].header.ofsProperties = getPos(bc_m2_file);
				fwrite(ptr->views[i].Properties, sizeof(Property),
						ptr->views[i].header.nProperties, bc_m2_file);
				align(bc_m2_file);
			}
			if (ptr->views[i].header.nSubmeshes > 0) {
				ptr->views[i].header.ofsSubmeshes = getPos(bc_m2_file);
				fwrite(ptr->views[i].Submeshes, sizeof(Submesh),
						ptr->views[i].header.nSubmeshes, bc_m2_file);
				align(bc_m2_file);
			}
			if (ptr->views[i].header.nTextureUnits > 0) {
				ptr->views[i].header.ofsTextureUnits = getPos(bc_m2_file);
				fwrite(ptr->views[i].TextureUnits, sizeof(TexUnit),
						ptr->views[i].header.nTextureUnits, bc_m2_file);
				align(bc_m2_file);
			}

		}

		fseek(bc_m2_file, ptr->header.ofsViews, SEEK_SET);
		for (i = 0; i < ptr->header.nViews; i++) {
			fwrite(&ptr->views[i].header, sizeof(ViewsHeader), 1, bc_m2_file);
		}
		fseek(bc_m2_file, 0, SEEK_END);

		return 0;
	}
	return -1;
}

void write_Vec3DAnimBlock(FILE *bc_m2_file, AnimationBlock *ptrBlock,
		Vec3D_SubBlock *ptrDataBlock) {
	if (ptrBlock->Ranges.n > 0) {
		ptrBlock->Ranges.ofs = getPos(bc_m2_file);
		int j;
		for (j = 0; j < ptrBlock->Ranges.n; j++) {
			fwrite(ptrDataBlock->ranges[j], sizeof(Range), 1, bc_m2_file);
		}
		align(bc_m2_file);
	}
	if (ptrBlock->Times.n > 0) {
		ptrBlock->Times.ofs = getPos(bc_m2_file);
		fwrite(ptrDataBlock->times, sizeof(uint32), ptrBlock->Times.n,
				bc_m2_file);
		align(bc_m2_file);
	}
	if (ptrBlock->Keys.n > 0) {
		ptrBlock->Keys.ofs = getPos(bc_m2_file);
		int j;
		for (j = 0; j < ptrBlock->Keys.n; j++) {
			fwrite(ptrDataBlock->keys[j], sizeof(Vec3D), 1, bc_m2_file);
		}
		align(bc_m2_file);
	}
}
void write_QuatAnimBlock(FILE *bc_m2_file, AnimationBlock *ptrBlock,
		Quat_SubBlock *ptrDataBlock) {
	if (ptrBlock->Ranges.n > 0) {
		ptrBlock->Ranges.ofs = getPos(bc_m2_file);
		int j;
		for (j = 0; j < ptrBlock->Ranges.n; j++) {
			fwrite(ptrDataBlock->ranges[j], sizeof(Range), 1, bc_m2_file);
		}
		align(bc_m2_file);
	}
	if (ptrBlock->Times.n > 0) {
		ptrBlock->Times.ofs = getPos(bc_m2_file);
		fwrite(ptrDataBlock->times, sizeof(uint32), ptrBlock->Times.n,
				bc_m2_file);
		align(bc_m2_file);
	}
	if (ptrBlock->Keys.n > 0) {
		ptrBlock->Keys.ofs = getPos(bc_m2_file);
		int j;
		for (j = 0; j < ptrBlock->Keys.n; j++) {
			fwrite(ptrDataBlock->keys[j], sizeof(Quat), 1, bc_m2_file);
		}
		align(bc_m2_file);
	}
}

/**
 * Write bones. Good example of writing a structure with Animations Blocks inside.
 * @param bc_m2_file The file to write data.
 * @param ptr A pointer to a M2/BC structure.
 */
int write_bones(FILE *bc_m2_file, BCM2 *ptr) {
	if (ptr->header.nBones > 0) {
		ptr->header.ofsBones = getPos(bc_m2_file);
		fwrite(ptr->bones, sizeof(ModelBoneDef), ptr->header.nBones,
				bc_m2_file);
		align(bc_m2_file);
		int i;
		for (i = 0; i < ptr->header.nBones; i++) {
			AnimationBlock *ptrBlock;
			Vec3D_SubBlock *Vec3D_ptrDataBlock;
			Quat_SubBlock *Quat_ptrDataBlock;

			//translation
			ptrBlock = &ptr->bones[i].trans;
			Vec3D_ptrDataBlock = &ptr->bonesdata[i].trans;
			write_Vec3DAnimBlock(bc_m2_file, ptrBlock, Vec3D_ptrDataBlock);

			//rotation
			ptrBlock = &ptr->bones[i].rot;
			Quat_ptrDataBlock = &ptr->bonesdata[i].rot;
			write_QuatAnimBlock(bc_m2_file, ptrBlock, Quat_ptrDataBlock);

			//scaling
			ptrBlock = &ptr->bones[i].scal;
			Vec3D_ptrDataBlock = &ptr->bonesdata[i].scal;
			write_Vec3DAnimBlock(bc_m2_file, ptrBlock, Vec3D_ptrDataBlock);
		}
	}
	fseek(bc_m2_file, ptr->header.ofsBones, SEEK_SET);
	fwrite(ptr->bones, sizeof(ModelBoneDef), ptr->header.nBones, bc_m2_file);
	fseek(bc_m2_file, 0, SEEK_END);
	return 0;
}

/**
 * Write a M2/BC file from its corresponding structure
 * @param bc_m2_file The file to write data.
 * @param ptr A pointer to a M2/BC structure.
 */
int write_model(FILE *bc_m2_file, BCM2 *ptr) {
	//Header
	fwrite(&ptr->header, sizeof(ModelHeader), 1, bc_m2_file);
	align(bc_m2_file);

	//Filename
	ptr->header.nameOfs = getPos(bc_m2_file);
	fwrite(ptr->filename, sizeof(char), ptr->header.nameLength, bc_m2_file);
	align(bc_m2_file);

	//Animations
	if (ptr->header.nAnimations > 0) {
		ptr->header.ofsAnimations = getPos(bc_m2_file);
		fwrite(ptr->animations, sizeof(ModelAnimation), ptr->header.nAnimations,
				bc_m2_file);
		align(bc_m2_file);
	}

	//Bones
	write_bones(bc_m2_file, ptr);

	//Animlookup
	if (ptr->header.nAnimationLookup > 0) {
		ptr->header.ofsAnimationLookup = getPos(bc_m2_file);
		fwrite(ptr->AnimLookup, sizeof(int16), ptr->header.nAnimationLookup,
				bc_m2_file);
		align(bc_m2_file);
	}

	//Keybonelookup
	if (ptr->header.nKeyBoneLookup > 0) {
		ptr->header.ofsKeyBoneLookup = getPos(bc_m2_file);
		fwrite(ptr->keybonelookup, sizeof(short), ptr->header.nKeyBoneLookup,
				bc_m2_file);
		align(bc_m2_file);
	}

	//Vertices
	if (ptr->header.nVertices > 0) {
		ptr->header.ofsVertices = getPos(bc_m2_file);
		fwrite(ptr->vertices, sizeof(ModelVertex), ptr->header.nVertices,
				bc_m2_file);
		align(bc_m2_file);
	}
	//Views
	write_views(bc_m2_file, ptr);

	//TODO Colors

	//Textures
	if (ptr->header.nTextures > 0) {
		ptr->header.ofsTextures = getPos(bc_m2_file);
		fwrite(ptr->textures_def, sizeof(ModelTextureDef),
				ptr->header.nTextures, bc_m2_file);
		align(bc_m2_file);
		int i;
		for (i = 0; i < ptr->header.nTextures; i++) {
			if (ptr->textures_def[i].type == 0) {
				ptr->textures_def[i].nameOfs = getPos(bc_m2_file);
				fwrite(ptr->texture_names[i], sizeof(char),
						ptr->textures_def[i].nameLen, bc_m2_file);
				align(bc_m2_file);
			}
		}

		fseek(bc_m2_file, ptr->header.ofsTextures, SEEK_SET);
		fwrite(ptr->textures_def, sizeof(ModelTextureDef),
				ptr->header.nTextures, bc_m2_file);
		fseek(bc_m2_file, 0, SEEK_END);
	}

	//RenderFlags
	if (ptr->header.nRenderFlags > 0) {
		ptr->header.ofsRenderFlags = getPos(bc_m2_file);
		fwrite(ptr->renderflags, sizeof(int), ptr->header.nRenderFlags,
				bc_m2_file);
		align(bc_m2_file);
	}

	//BoneLookupTable
	if (ptr->header.nBoneLookupTable > 0) {
		ptr->header.ofsBoneLookupTable = getPos(bc_m2_file);
		fwrite(ptr->BoneLookupTable, sizeof(int16),
				ptr->header.nBoneLookupTable, bc_m2_file);
		align(bc_m2_file);
	}

	//Lookups
	if (ptr->header.nTexLookup > 0) {
		ptr->header.ofsTexLookup = getPos(bc_m2_file);
		fwrite(ptr->TexLookupTable, sizeof(short), ptr->header.nTexLookup,
				bc_m2_file);
		align(bc_m2_file);
	}
	if (ptr->header.nTexUnitLookup > 0) {
		ptr->header.ofsTexUnitLookup = getPos(bc_m2_file);
		fwrite(ptr->TexUnit, sizeof(short), ptr->header.nTexUnitLookup,
				bc_m2_file);
		align(bc_m2_file);
	}
	if (ptr->header.nTransparencyLookup > 0) {
		ptr->header.ofsTransparencyLookup = getPos(bc_m2_file);
		fwrite(ptr->TransparencyLookup, sizeof(short),
				ptr->header.nTransparencyLookup, bc_m2_file);
		align(bc_m2_file);
	}
	if (ptr->header.nTexAnimLookup > 0) {
		ptr->header.ofsTexAnimLookup = getPos(bc_m2_file);
		fwrite(ptr->TexAnimLookup, sizeof(short), ptr->header.nTexAnimLookup,
				bc_m2_file);
		align(bc_m2_file);
	}

	//TODO Attachments

	if (ptr->header.nAttachLookup > 0) {
		ptr->header.ofsAttachLookup = getPos(bc_m2_file);
		fwrite(ptr->AttachLookup, sizeof(int16), ptr->header.nAttachLookup,
				bc_m2_file);
		align(bc_m2_file);
	}

	//Rewrite the header with updated offsets
	fseek(bc_m2_file, 0, SEEK_SET);
	fwrite(&ptr->header, sizeof(ModelHeader), 1, bc_m2_file);

	return 0;
}
