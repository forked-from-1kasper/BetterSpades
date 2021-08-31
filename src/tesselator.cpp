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
#include <assert.h>

#include <common.hpp>
#include <tesselator.hpp>

static size_t vertex_type_size(enum tesselator_vertex_type type) {
    switch(type) {
        case VERTEX_INT: return sizeof(int16_t);
        case VERTEX_FLOAT: return sizeof(float);
        default: return 0;
    }
}

void tesselator_create(struct tesselator* t, enum tesselator_vertex_type type, int has_normal) {
    t->quad_count = 0;
    t->quad_space = 128;
    t->vertices = NULL;
    t->colors = NULL;
    t->vertex_type = type;
    t->has_normal = has_normal;

#ifdef TESSELATE_QUADS
    t->vertices = malloc(t->quad_space * vertex_type_size(t->vertex_type) * 3 * 4);
    CHECK_ALLOCATION_ERROR(t->vertices)
    t->colors = (uint32_t*) malloc(t->quad_space * sizeof(uint32_t) * 4);
    CHECK_ALLOCATION_ERROR(t->colors)

    if(t->has_normal) {
        t->normals = (int8_t*) malloc(t->quad_space * sizeof(int8_t) * 3 * 4);
        CHECK_ALLOCATION_ERROR(t->normals)
    } else {
        t->normals = NULL;
    }
#endif

#ifdef TESSELATE_TRIANGLES
    t->vertices = malloc(t->quad_space * vertex_type_size(t->vertex_type) * 3 * 6);
    CHECK_ALLOCATION_ERROR(t->vertices)
    t->colors = malloc(t->quad_space * sizeof(uint32_t) * 6);
    CHECK_ALLOCATION_ERROR(t->colors)

    if(t->has_normal) {
        t->normals = malloc(t->quad_space * sizeof(int8_t) * 3 * 6);
        CHECK_ALLOCATION_ERROR(t->normals)
    } else {
        t->normals = NULL;
    }
#endif
}

void tesselator_clear(struct tesselator* t) {
    t->quad_count = 0;
}

void tesselator_free(struct tesselator* t) {
    if(t->vertices) {
        free(t->vertices);
        t->vertices = NULL;
    }

    if(t->colors) {
        free(t->colors);
        t->colors = NULL;
    }

    if(t->normals) {
        free(t->normals);
        t->normals = NULL;
    }
}

void tesselator_draw(struct tesselator* t, int with_color) {
    glEnableClientState(GL_VERTEX_ARRAY);

    if(t->has_normal) {
        glEnableClientState(GL_NORMAL_ARRAY);
        glNormalPointer(GL_BYTE, 0, t->normals);
    }

    switch(t->vertex_type) {
        case VERTEX_INT: glVertexPointer(3, GL_SHORT, 0, t->vertices); break;
        case VERTEX_FLOAT: glVertexPointer(3, GL_FLOAT, 0, t->vertices); break;
    }

    if(with_color) {
        glEnableClientState(GL_COLOR_ARRAY);
        glColorPointer(4, GL_UNSIGNED_BYTE, 0, t->colors);
    }

#ifdef TESSELATE_QUADS
    glDrawArrays(GL_QUADS, 0, t->quad_count * 4);
#endif

#ifdef TESSELATE_TRIANGLES
    glDrawArrays(GL_TRIANGLES, 0, t->quad_count * 6);
#endif

    if(with_color) {
        glDisableClientState(GL_COLOR_ARRAY);
    }

    glDisableClientState(GL_VERTEX_ARRAY);

    if(t->has_normal) {
        glDisableClientState(GL_NORMAL_ARRAY);
    }
}

void tesselator_glx(struct tesselator* t, struct glx_displaylist* x) {
#ifdef TESSELATE_QUADS
    switch(t->vertex_type) {
        case VERTEX_INT:
            glx_displaylist_update(x, t->quad_count * 4, GLX_DISPLAYLIST_NORMAL, t->colors, t->vertices, t->normals);
            break;
        case VERTEX_FLOAT:
            glx_displaylist_update(x, t->quad_count * 4, GLX_DISPLAYLIST_ENHANCED, t->colors, t->vertices, t->normals);
            break;
    }
#endif

#ifdef TESSELATE_TRIANGLES
    switch(t->vertex_type) {
        case VERTEX_INT:
            glx_displaylist_update(x, t->quad_count * 6, GLX_DISPLAYLIST_NORMAL, t->colors, t->vertices, t->normals);
            break;
        case VERTEX_FLOAT:
            glx_displaylist_update(x, t->quad_count * 6, GLX_DISPLAYLIST_ENHANCED, t->colors, t->vertices, t->normals);
            break;
    }
#endif
}

void tesselator_set_color(struct tesselator* t, uint32_t color) {
    t->color = color;
}

void tesselator_set_normal(struct tesselator* t, int8_t x, int8_t y, int8_t z) {
    t->normal[0] = x;
    t->normal[1] = y;
    t->normal[2] = z;
}

static void tesselator_check_space(struct tesselator* t) {
    if(t->quad_count >= t->quad_space) {
        t->quad_space *= 2;

#ifdef TESSELATE_QUADS
        t->vertices = realloc(t->vertices, t->quad_space * vertex_type_size(t->vertex_type) * 3 * 4);
        CHECK_ALLOCATION_ERROR(t->vertices)
        t->colors = (uint32_t*) realloc(t->colors, t->quad_space * sizeof(uint32_t) * 4);
        CHECK_ALLOCATION_ERROR(t->colors)

        if(t->has_normal) {
            t->normals = (int8_t*) realloc(t->normals, t->quad_space * sizeof(int8_t) * 3 * 4);
            CHECK_ALLOCATION_ERROR(t->normals)
        }
#endif

#ifdef TESSELATE_TRIANGLES
        t->vertices = realloc(t->vertices, t->quad_space * vertex_type_size(t->vertex_type) * 3 * 6);
        CHECK_ALLOCATION_ERROR(t->vertices)
        t->colors = realloc(t->colors, t->quad_space * sizeof(uint32_t) * 6);
        CHECK_ALLOCATION_ERROR(t->colors)

        if(t->has_normal) {
            t->normals = realloc(t->normals, t->quad_space * sizeof(int8_t) * 3 * 6);
            CHECK_ALLOCATION_ERROR(t->normals)
        }
#endif
    }
}

static void tesselator_emit_color(struct tesselator* t, uint32_t* colors) {
#ifdef TESSELATE_QUADS
    memcpy(t->colors + t->quad_count * 4, colors, sizeof(uint32_t) * 4);
#endif

#ifdef TESSELATE_TRIANGLES
    memcpy(t->colors + t->quad_count * 6, colors, sizeof(uint32_t) * 3);
    t->colors[t->quad_count * 6 + 3] = colors[0];
    memcpy(t->colors + t->quad_count * 6 + 4, colors + 2, sizeof(uint32_t) * 2);
#endif
}

static void tesselator_emit_normals(struct tesselator* t, int8_t* normals) {
    if(t->has_normal) {
#ifdef TESSELATE_QUADS
        memcpy(t->normals + t->quad_count * 3 * 4, normals, sizeof(int8_t) * 3 * 4);
#endif

#ifdef TESSELATE_TRIANGLES
        memcpy(t->normals + t->quad_count * 3 * 6 + 3 * 0, normals, sizeof(int8_t) * 3 * 3);
        memcpy(t->normals + t->quad_count * 3 * 6 + 3 * 3, normals + 3 * 0, sizeof(int8_t) * 3);
        memcpy(t->normals + t->quad_count * 3 * 6 + 3 * 4, normals + 3 * 2, sizeof(int8_t) * 3 * 2);
#endif
    }
}

void tesselator_addi(struct tesselator* t, int16_t* coords, uint32_t* colors, int8_t* normals) {
    assert(t->vertex_type == VERTEX_INT);

    tesselator_check_space(t);
    tesselator_emit_color(t, colors);
    tesselator_emit_normals(t, normals);

#ifdef TESSELATE_QUADS
    memcpy(((int16_t*)t->vertices) + t->quad_count * 3 * 4, coords, sizeof(int16_t) * 3 * 4);
#endif

#ifdef TESSELATE_TRIANGLES
    memcpy(((int16_t*)t->vertices) + t->quad_count * 3 * 6 + 3 * 0, coords, sizeof(int16_t) * 3 * 3);
    memcpy(((int16_t*)t->vertices) + t->quad_count * 3 * 6 + 3 * 3, coords + 3 * 0, sizeof(int16_t) * 3);
    memcpy(((int16_t*)t->vertices) + t->quad_count * 3 * 6 + 3 * 4, coords + 3 * 2, sizeof(int16_t) * 3 * 2);
#endif

    t->quad_count++;
}

void tesselator_addf(struct tesselator* t, float* coords, uint32_t* colors, int8_t* normals) {
    assert(t->vertex_type == VERTEX_FLOAT);

    tesselator_check_space(t);
    tesselator_emit_color(t, colors);
    tesselator_emit_normals(t, normals);

#ifdef TESSELATE_QUADS
    memcpy(((float*)t->vertices) + t->quad_count * 3 * 4, coords, sizeof(float) * 3 * 4);
#endif

#ifdef TESSELATE_TRIANGLES
    memcpy(((float*)t->vertices) + t->quad_count * 3 * 6 + 3 * 0, coords, sizeof(float) * 3 * 3);
    memcpy(((float*)t->vertices) + t->quad_count * 3 * 6 + 3 * 3, coords + 3 * 0, sizeof(float) * 3);
    memcpy(((float*)t->vertices) + t->quad_count * 3 * 6 + 3 * 4, coords + 3 * 2, sizeof(float) * 3 * 2);
#endif

    t->quad_count++;
}

void tesselator_addi_simple(struct tesselator* t, int16_t* coords) {
    int8_t normal[] {t->normal[0], t->normal[1], t->normal[2], t->normal[0], t->normal[1],
                     t->normal[2], t->normal[0], t->normal[1], t->normal[2], t->normal[0],
                     t->normal[1], t->normal[2]};
    uint32_t colors[] {t->color, t->color, t->color, t->color};

    tesselator_addi(t, coords, (uint32_t*) &colors, t->has_normal ? (int8_t*) &normal : NULL);
}

void tesselator_addf_simple(struct tesselator* t, float* coords) {
    int8_t normal[] {t->normal[0], t->normal[1], t->normal[2], t->normal[0], t->normal[1],
                     t->normal[2], t->normal[0], t->normal[1], t->normal[2], t->normal[0],
                     t->normal[1], t->normal[2]};
    uint32_t colors[] {t->color, t->color, t->color, t->color};

    tesselator_addf(t, coords, (uint32_t*) &colors, t->has_normal ? (int8_t*) &normal : NULL);
}

void tesselator_addi_cube_face_adv(struct tesselator* t, enum tesselator_cube_face face, int16_t x, int16_t y,
                                   int16_t z, int16_t sx, int16_t sy, int16_t sz) {
    switch(face) {
        case CUBE_FACE_Z_N: {
            int16_t face[] {x, y, z, x, y + sy, z, x + sx, y + sy, z, x + sx, y, z};
            tesselator_addi_simple(t, (int16_t*) &face);
            break;
        }
        case CUBE_FACE_Z_P: {
            int16_t face[] {x, y, z + sz, x + sx, y, z + sz, x + sx, y + sy, z + sz, x, y + sy, z + sz};
            tesselator_addi_simple(t, (int16_t*) &face);
            break;
        }
        case CUBE_FACE_X_N: {
            int16_t face[] {x, y, z, x, y, z + sz, x, y + sy, z + sz, x, y + sy, z};
            tesselator_addi_simple(t, (int16_t*) &face);
            break;
        }
        case CUBE_FACE_X_P: {
            int16_t face[] {x + sx, y, z, x + sx, y + sy, z, x + sx, y + sy, z + sz, x + sx, y, z + sz};
            tesselator_addi_simple(t, (int16_t*) &face);
            break;
        }
        case CUBE_FACE_Y_P: {
            int16_t face[] {x, y + sy, z, x, y + sy, z + sz, x + sx, y + sy, z + sz, x + sx, y + sy, z};
            tesselator_addi_simple(t, (int16_t*) &face);
            break;
        }
        case CUBE_FACE_Y_N: {
            int16_t face[] {x, y, z, x + sx, y, z, x + sx, y, z + sz, x, y, z + sz};
            tesselator_addi_simple(t, (int16_t*) &face);
            break;
        }
    }
}

void tesselator_addi_cube_face(struct tesselator* t, enum tesselator_cube_face face, int16_t x, int16_t y, int16_t z) {
    tesselator_addi_cube_face_adv(t, face, x, y, z, 1, 1, 1);
}

void tesselator_addf_cube_face(struct tesselator* t, enum tesselator_cube_face face, float x, float y, float z,
                               float sz) {
    switch(face) {
        case CUBE_FACE_Z_N: {
            float face[] {x, y, z, x, y + sz, z, x + sz, y + sz, z, x + sz, y, z};
            tesselator_addf_simple(t, (float*) face);
            break;
        }
        case CUBE_FACE_Z_P: {
            float face[] {x, y, z + sz, x + sz, y, z + sz, x + sz, y + sz, z + sz, x, y + sz, z + sz};
            tesselator_addf_simple(t, (float*) face);
            break;
        }
        case CUBE_FACE_X_N: {
            float face[] {x, y, z, x, y, z + sz, x, y + sz, z + sz, x, y + sz, z};
            tesselator_addf_simple(t, (float*) face);
            break;
        }
        case CUBE_FACE_X_P: {
            float face[] {x + sz, y, z, x + sz, y + sz, z, x + sz, y + sz, z + sz, x + sz, y, z + sz};
            tesselator_addf_simple(t, (float*) face);
            break;
        }
        case CUBE_FACE_Y_P: {
            float face[] {x, y + sz, z, x, y + sz, z + sz, x + sz, y + sz, z + sz, x + sz, y + sz, z};
            tesselator_addf_simple(t, (float*) face);
            break;
        }
        case CUBE_FACE_Y_N: {
            float face[] {x, y, z, x + sz, y, z, x + sz, y, z + sz, x, y, z + sz};
            tesselator_addf_simple(t, (float*) face);
            break;
        }
    }
}
