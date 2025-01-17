/*
    Copyright (c) 2017-2020 ByteBit

    This file is part of BetterSpades.

    BetterSpades is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    BetterSpades is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with BetterSpades.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <stdlib.h>
#include <string.h>
#include <math.h>

#include <common.hpp>
#include <player.hpp>
#include <file.hpp>
#include <camera.hpp>
#include <matrix.hpp>
#include <log.hpp>
#include <map.hpp>
#include <config.hpp>
#include <model.hpp>
#include <model_normals.hpp>
#include <texture.hpp>

kv6_t model_playerdead;
kv6_t model_playerhead;
kv6_t model_playertorso;
kv6_t model_playertorsoc;
kv6_t model_playerarms;
kv6_t model_playerleg;
kv6_t model_playerlegc;
kv6_t model_intel;
kv6_t model_tent;

kv6_t model_semi;
kv6_t model_semi_rear;
kv6_t model_semi_sight;
kv6_t model_semi_tracer;
kv6_t model_semi_casing;

kv6_t model_smg;
kv6_t model_smg_rear1;
kv6_t model_smg_rear2;
kv6_t model_smg_sight;
kv6_t model_smg_tracer;
kv6_t model_smg_casing;

kv6_t model_shotgun;
kv6_t model_shotgun_rear;
kv6_t model_shotgun_sight;
kv6_t model_shotgun_tracer;
kv6_t model_shotgun_casing;

kv6_t model_spade;
kv6_t model_block;
kv6_t model_grenade;

static void kv6_load_file(struct kv6_t* kv6, const char* filename, float scale) {
    void* data = file_load(filename);
    kv6_load(kv6, data, scale);
    free(data);
}

static void kv6_check_dimensions(struct kv6_t* kv6, float max) {
    if(maxc(maxc(kv6->xsiz, kv6->ysiz), kv6->zsiz) * kv6->scale > max) {
        log_error("Model dimensions too large");
        kv6->voxel_count = 0;
    }
}

void kv6_init() {
    kv6_load_file(&model_playerdead, "kv6/player/dead.kv6", 0.1F);
    kv6_load_file(&model_playerhead, "kv6/player/head.kv6", 0.1F);
    kv6_load_file(&model_playertorso, "kv6/player/torso.kv6", 0.1F);
    kv6_load_file(&model_playertorsoc, "kv6/player/torsoc.kv6", 0.1F);
    kv6_load_file(&model_playerarms, "kv6/player/arms.kv6", 0.1F);
    kv6_load_file(&model_playerleg, "kv6/player/leg.kv6", 0.1F);
    kv6_load_file(&model_playerlegc, "kv6/player/legc.kv6", 0.1F);

    kv6_load_file(&model_intel, "kv6/intel.kv6", 0.2F);
    kv6_load_file(&model_tent, "kv6/cp.kv6", 0.278F);

    kv6_load_file(&model_semi, "kv6/semi.kv6", 0.05F);
    kv6_load_file(&model_semi_tracer, "kv6/weapon/rifle/tracer.kv6", 0.05F);
    kv6_load_file(&model_semi_casing, "kv6/weapon/rifle/casing.kv6", 0.0125F);
    kv6_load_file(&model_semi_rear, "kv6/weapon/rifle/rear.kv6", 0.0025F);
    kv6_load_file(&model_semi_sight, "kv6/weapon/rifle/sight.kv6", 0.005F);

    kv6_load_file(&model_smg, "kv6/smg.kv6", 0.05F);
    kv6_load_file(&model_smg_tracer, "kv6/weapon/smg/tracer.kv6", 0.05F);
    kv6_load_file(&model_smg_casing, "kv6/weapon/smg/casing.kv6", 0.0125F);
    kv6_load_file(&model_smg_rear1, "kv6/weapon/smg/rear-1.kv6", 0.1F);
    kv6_load_file(&model_smg_rear2, "kv6/weapon/smg/rear-2.kv6", 0.05F);
    kv6_load_file(&model_smg_sight, "kv6/weapon/smg/sight.kv6", 0.08F);

    kv6_load_file(&model_shotgun, "kv6/shotgun.kv6", 0.05F);
    kv6_load_file(&model_shotgun_tracer, "kv6/weapon/shotgun/tracer.kv6", 0.05F);
    kv6_load_file(&model_shotgun_casing, "kv6/weapon/shotgun/casing.kv6", 0.0125F);
    kv6_load_file(&model_shotgun_rear, "kv6/weapon/shotgun/rear.kv6", 0.05F);
    kv6_load_file(&model_shotgun_sight, "kv6/weapon/shotgun/sight.kv6", 0.05F);

    kv6_load_file(&model_spade, "kv6/spade.kv6", 0.05F);
    kv6_load_file(&model_block, "kv6/block.kv6", 0.05F);
    model_block.colorize = true;
    kv6_load_file(&model_grenade, "kv6/grenade.kv6", 0.05F);

    kv6_check_dimensions(&model_playerhead, 1.2F);
    kv6_check_dimensions(&model_playertorso, 1.8F);
    kv6_check_dimensions(&model_playertorsoc, 1.6F);
    kv6_check_dimensions(&model_playerarms, 2.0F);
    kv6_check_dimensions(&model_playerleg, 2.0F);
    kv6_check_dimensions(&model_playerlegc, 1.6F);

    kv6_check_dimensions(&model_semi, 2.25F);
    kv6_check_dimensions(&model_smg, 2.25F);
    kv6_check_dimensions(&model_shotgun, 2.25F);
    kv6_check_dimensions(&model_spade, 2.25F);
    kv6_check_dimensions(&model_block, 2.25F);
    kv6_check_dimensions(&model_grenade, 2.25F);
}

void kv6_rebuild_complete() {
    kv6_rebuild(&model_playerdead);
    kv6_rebuild(&model_playerhead);
    kv6_rebuild(&model_playertorso);
    kv6_rebuild(&model_playertorsoc);
    kv6_rebuild(&model_playerarms);
    kv6_rebuild(&model_playerleg);
    kv6_rebuild(&model_playerlegc);
    kv6_rebuild(&model_intel);
    kv6_rebuild(&model_tent);
    kv6_rebuild(&model_semi);
    kv6_rebuild(&model_smg);
    kv6_rebuild(&model_shotgun);
    kv6_rebuild(&model_spade);
    kv6_rebuild(&model_block);
    kv6_rebuild(&model_grenade);
    kv6_rebuild(&model_semi_tracer);
    kv6_rebuild(&model_smg_tracer);
    kv6_rebuild(&model_shotgun_tracer);
    kv6_rebuild(&model_semi_casing);
    kv6_rebuild(&model_smg_casing);
    kv6_rebuild(&model_shotgun_casing);
}

void kv6_load(struct kv6_t* kv6, void* ptr, float scale) {
    kv6->colorize = false;
    kv6->has_display_list = false;
    kv6->scale = scale;

    size_t index = 0;

    auto bytes = (unsigned char*) ptr;
    if(buffer_read32(bytes, index) == 0x6C78764B) { //"Kvxl"
        index += 4;
        kv6->xsiz = buffer_read32(bytes, index);
        index += 4;
        kv6->ysiz = buffer_read32(bytes, index);
        index += 4;
        kv6->zsiz = buffer_read32(bytes, index);
        index += 4;

        kv6->xpiv = buffer_readf(bytes, index);
        index += 4;
        kv6->ypiv = buffer_readf(bytes, index);
        index += 4;
        kv6->zpiv = kv6->zsiz - buffer_readf(bytes, index);
        index += 4;

        kv6->voxel_count = buffer_read32(bytes, index);
        index += 4;

        kv6->voxels = (kv6_voxel*) malloc(sizeof(struct kv6_voxel) * kv6->voxel_count);
        CHECK_ALLOCATION_ERROR(kv6->voxels)

        for(size_t k = 0; k < kv6->voxel_count; k++) {
            uint32_t color = buffer_read32(bytes, index);
            index += 4;
            uint16_t zpos = buffer_read16(bytes, index);
            index += 2;
            uint8_t visfaces = buffer_read8(bytes, index++); // 0x00zZyYxX
            uint8_t lighting = buffer_read8(bytes, index++); // compressed normal vector (also referred to as lighting)

            kv6->voxels[k] = (struct kv6_voxel) {
                .x = 0, .y = 0, .z = (kv6->zsiz - 1) - zpos,
                .visfaces = visfaces, .color = (color & 0xFFFFFF) | (lighting << 24),
            };
        }

        index += 4 * kv6->xsiz;

        struct kv6_voxel* voxel = kv6->voxels;

        for(size_t x = 0; x < kv6->xsiz; x++) {
            for(size_t y = 0; y < kv6->ysiz; y++) {
                uint16_t size = buffer_read16(bytes, index);
                index += 2;

                for(size_t z = 0; z < size; z++, voxel++) {
                    voxel->x = x;
                    voxel->y = y;
                }
            }
        }
    } else {
        log_error("Data not in kv6 format");
        kv6->xsiz = kv6->ysiz = kv6->zsiz = 0;
        kv6->voxel_count = 0;
    }
}

void kv6_rebuild(struct kv6_t* kv6) {
    if(kv6->has_display_list) {
        glx_displaylist_destroy(kv6->display_list + 0);
        glx_displaylist_destroy(kv6->display_list + 1);
        kv6->has_display_list = false;
    }
}

void kv6_calclight(int x, int y, int z) {
    float f = 1.0F;

    if(x >= 0 && y >= 0 && z >= 0)
        f = map_sunblock(x, y, z);

    float lambient[4] = {0.5F * f, 0.5F * f, 0.5F * f, 1.0F};
    float ldiffuse[4] = {0.5F * f, 0.5F * f, 0.5F * f, 1.0F};

    glLightfv(GL_LIGHT0, GL_AMBIENT, lambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, ldiffuse);
}

static int kv6_voxel_cmp(const void* a, const void* b) {
    auto A = (const struct kv6_voxel*) a;
    auto B = (const struct kv6_voxel*) b;

    if(A->x == B->x) {
        if(A->y == B->y) {
            return B->z - A->z;
        } else {
            return A->y - B->y;
        }
    } else {
        return A->x - B->x;
    }
}

static void greedy_mesh(struct kv6_t* kv6, struct kv6_voxel* voxel, uint8_t* marked, size_t* max_a, size_t* max_b,
                        uint8_t face) {
    if(!settings.greedy_meshing) {
        *max_a = 1;
        *max_b = 1;
    } else {
        switch(face) {
            case KV6_VIS_POS_X:
            case KV6_VIS_NEG_X:
                *max_a = kv6->ysiz;
                *max_b = kv6->zsiz;
                break;
            case KV6_VIS_POS_Y:
            case KV6_VIS_NEG_Y:
                *max_a = kv6->xsiz;
                *max_b = kv6->ysiz;
                break;
            case KV6_VIS_POS_Z:
            case KV6_VIS_NEG_Z:
                *max_a = kv6->xsiz;
                *max_b = kv6->zsiz;
                break;
        }

        struct kv6_voxel lookup = *voxel;

        for(size_t a = 0; a < *max_a; a++) {
            struct kv6_voxel* recent[*max_b];
            size_t b;
            for(b = 0; b < *max_b; b++) {
                switch(face) {
                    case KV6_VIS_POS_X:
                    case KV6_VIS_NEG_X:
                        lookup.y = voxel->y + b;
                        lookup.z = voxel->z - a;
                        break;
                    case KV6_VIS_POS_Y:
                    case KV6_VIS_NEG_Y:
                        lookup.x = voxel->x + a;
                        lookup.y = voxel->y + b;
                        break;
                    case KV6_VIS_POS_Z:
                    case KV6_VIS_NEG_Z:
                        lookup.x = voxel->x + a;
                        lookup.z = voxel->z - b;
                        break;
                }

                auto neighbour = (kv6_voxel*) bsearch(&lookup, kv6->voxels, kv6->voxel_count, sizeof(kv6_voxel), kv6_voxel_cmp);

                if(!neighbour || !(neighbour->visfaces & face)
                   || (neighbour->color & 0xFFFFFF) != (voxel->color & 0xFFFFFF)
                   || marked[neighbour - kv6->voxels] & face) {
                    break;
                } else {
                    marked[neighbour - kv6->voxels] |= face;
                    recent[b] = neighbour;
                }
            }

            if(a == 0)
                *max_b = b;

            if(b < *max_b) { // early abort
                *max_a = a;

                for(size_t k = 0; k < b; k++)
                    marked[recent[k] - kv6->voxels] &= ~face;

                break;
            }
        }
    }
}

static int kv6_program = -1;
void kv6_render(struct kv6_t* kv6, unsigned char team) {
    if(!kv6) return;
    if(team == TEAM_SPECTATOR) team = 2;
    if (!kv6->has_display_list) {
        struct tesselator tess_color;
        tesselator_create(&tess_color, VERTEX_INT, 1);
        struct tesselator tess_team;
        tesselator_create(&tess_team, VERTEX_INT, 1);

        glx_displaylist_create(kv6->display_list + 0, true, true);
        glx_displaylist_create(kv6->display_list + 1, true, true);

        uint8_t marked[kv6->voxel_count];
        memset(marked, 0, sizeof(uint8_t) * kv6->voxel_count);

        struct kv6_voxel* voxel = kv6->voxels;
        for (size_t k = 0; k < kv6->voxel_count; k++, voxel++) {
            int b = red(voxel->color);
            int g = green(voxel->color);
            int r = blue(voxel->color);
            int a = alpha(voxel->color);

            struct tesselator* tess = &tess_color;

            if ((r | g | b) == 0) {
                tess = &tess_team;
                r = g = b = 255;
            } else if(kv6->colorize) {
                r = g = b = 255;
            }

            tesselator_set_normal(tess, kv6_normals[a][0] * 128, -kv6_normals[a][2] * 128, kv6_normals[a][1] * 128);

            if (voxel->visfaces & KV6_VIS_POS_Y) {
                size_t max_x, max_z;
                greedy_mesh(kv6, voxel, marked, &max_x, &max_z, KV6_VIS_POS_Y);

                tesselator_set_color(tess, rgba(r, g, b, 0));
                tesselator_addi_cube_face_adv(tess, CUBE_FACE_Y_P, voxel->x, voxel->z, voxel->y, max_x, 1, max_z);
            }

            if (voxel->visfaces & KV6_VIS_NEG_Y) {
                size_t max_x, max_z;
                greedy_mesh(kv6, voxel, marked, &max_x, &max_z, KV6_VIS_NEG_Y);

                tesselator_set_color(tess, rgba(r * 0.6F, g * 0.6F, b * 0.6F, 0));
                tesselator_addi_cube_face_adv(tess, CUBE_FACE_Y_N, voxel->x, voxel->z, voxel->y, max_x, 1, max_z);
            }

            if (voxel->visfaces & KV6_VIS_NEG_Z) {
                size_t max_x, max_y;
                greedy_mesh(kv6, voxel, marked, &max_x, &max_y, KV6_VIS_NEG_Z);

                tesselator_set_color(tess, rgba(r * 0.95F, g * 0.95F, b * 0.95F, 0));
                tesselator_addi_cube_face_adv(tess, CUBE_FACE_Z_N, voxel->x, voxel->z - (max_y - 1), voxel->y,
                                              max_x, max_y, 1);
            }

            if (voxel->visfaces & KV6_VIS_POS_Z) {
                size_t max_x, max_y;
                greedy_mesh(kv6, voxel, marked, &max_x, &max_y, KV6_VIS_POS_Z);

                tesselator_set_color(tess, rgba(r * 0.9F, g * 0.9F, b * 0.9F, 0));
                tesselator_addi_cube_face_adv(tess, CUBE_FACE_Z_P, voxel->x, voxel->z - (max_y - 1), voxel->y,
                                              max_x, max_y, 1);
            }

            if(voxel->visfaces & KV6_VIS_NEG_X) {
                size_t max_y, max_z;
                greedy_mesh(kv6, voxel, marked, &max_y, &max_z, KV6_VIS_NEG_X);

                tesselator_set_color(tess, rgba(r * 0.85F, g * 0.85F, b * 0.85F, 0));
                tesselator_addi_cube_face_adv(tess, CUBE_FACE_X_N, voxel->x, voxel->z - (max_y - 1), voxel->y, 1,
                                              max_y, max_z);
            }

            if(voxel->visfaces & KV6_VIS_POS_X) {
                size_t max_y, max_z;
                greedy_mesh(kv6, voxel, marked, &max_y, &max_z, KV6_VIS_POS_X);

                tesselator_set_color(tess, rgba(r * 0.8F, g * 0.8F, b * 0.8F, 0));
                tesselator_addi_cube_face_adv(tess, CUBE_FACE_X_P, voxel->x, voxel->z - (max_y - 1), voxel->y, 1,
                                              max_y, max_z);
            }
        }

        tesselator_glx(&tess_color, kv6->display_list + 0);
        tesselator_glx(&tess_team, kv6->display_list + 1);

        tesselator_free(&tess_color);
        tesselator_free(&tess_team);

        kv6->has_display_list = true;
    } else {
        glEnable(GL_LIGHTING);
        glEnable(GL_LIGHT0);
        glEnable(GL_COLOR_MATERIAL);
        glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
        glEnable(GL_NORMALIZE);

        glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE);
        glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_RGB, GL_MODULATE);
        glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_ALPHA, GL_MODULATE);
        glTexEnvi(GL_TEXTURE_ENV, GL_SRC0_RGB, GL_CONSTANT);
        glTexEnvi(GL_TEXTURE_ENV, GL_SRC0_ALPHA, GL_CONSTANT);
        glTexEnvi(GL_TEXTURE_ENV, GL_SRC1_RGB, GL_PREVIOUS);
        glTexEnvi(GL_TEXTURE_ENV, GL_SRC1_ALPHA, GL_PREVIOUS);
        glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND0_RGB, GL_SRC_COLOR);
        glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND0_ALPHA, GL_SRC_ALPHA);
        glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND1_RGB, GL_SRC_COLOR);
        glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND1_ALPHA, GL_SRC_ALPHA);
        glBindTexture(GL_TEXTURE_2D, texture_dummy.texture_id);

        if (kv6->colorize) {
            glEnable(GL_TEXTURE_2D);
            float color[] {kv6->red, kv6->green, kv6->blue, 1.0F};
            glTexEnvfv(GL_TEXTURE_ENV, GL_TEXTURE_ENV_COLOR, (float*) &color);
        }

        glScalef(kv6->scale, kv6->scale, kv6->scale);
        glTranslatef(-kv6->xpiv, -kv6->zpiv, -kv6->ypiv);

        glx_displaylist_draw(kv6->display_list + 0, GLX_DISPLAYLIST_NORMAL);

        if (!kv6->colorize) glEnable(GL_TEXTURE_2D);

        switch (team) {
            case TEAM_1: {
                float color[] {gamestate.team_1.red * 0.75F / 255.0F,
                               gamestate.team_1.green * 0.75F / 255.0F,
                               gamestate.team_1.blue * 0.75F / 255.0F, 1.0F};
                glTexEnvfv(GL_TEXTURE_ENV, GL_TEXTURE_ENV_COLOR, (float*) &color);
                break;
            }
            case TEAM_2: {
                float color[] {gamestate.team_2.red * 0.75F / 255.0F,
                               gamestate.team_2.green * 0.75F / 255.0F,
                               gamestate.team_2.blue * 0.75F / 255.0F, 1.0F};
                glTexEnvfv(GL_TEXTURE_ENV, GL_TEXTURE_ENV_COLOR, (float*) &color);
                break;
            }
            default: {
                float color[] {0, 0, 0, 1};
                glTexEnvfv(GL_TEXTURE_ENV, GL_TEXTURE_ENV_COLOR, (float*) &color);
            }
        }

        glx_displaylist_draw(kv6->display_list + 1, GLX_DISPLAYLIST_NORMAL);

        glBindTexture(GL_TEXTURE_2D, 0);
        glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
        glDisable(GL_TEXTURE_2D);

        glDisable(GL_NORMALIZE);
        glDisable(GL_COLOR_MATERIAL);
        glDisable(GL_LIGHT0);
        glDisable(GL_LIGHTING);
    }
}
