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
#include <math.h>
#include <string.h>

#include <common.hpp>
#include <matrix.hpp>
#include <window.hpp>
#include <tracer.hpp>
#include <model.hpp>
#include <camera.hpp>
#include <texture.hpp>
#include <config.hpp>
#include <sound.hpp>
#include <entitysystem.hpp>

struct entity_system tracers;

void tracer_pvelocity(float* o, struct Player* p) {
    o[0] = o[0] * 256.0F / 32.0F + p->physics.velocity.x;
    o[1] = o[1] * 256.0F / 32.0F + p->physics.velocity.y;
    o[2] = o[2] * 256.0F / 32.0F + p->physics.velocity.z;
}

void tracer_add(int type, float x0, float y0, float z0, float dx, float dy, float dz) {
    auto x = x0 + dx / 4.0F; auto y = y0 + dy / 4.0F; auto z = z0 + dz / 4.0F;

    Tracer t {
        .x = x, .y = y, .z = z,
        .r = {.origin = {x, y, z}, .direction = {dx, dy, dz}},
        .type = type, .created = window_time(),
    };

    float len = len3D(dx, dy, dz);
    camera_hit(&t.hit, -1, t.x, t.y, t.z, dx / len, dy / len, dz / len, 128.0F);

    entitysys_add(&tracers, &t);
}

static bool tracer_render_single(void* obj, void* user) {
    auto t = (Tracer*) obj; mat4 matrix_tracer;

    matrix_load(matrix_tracer, matrix_model);
    matrix_translate(matrix_tracer, t->r.origin.x, t->r.origin.y, t->r.origin.z);
    matrix_pointAt(matrix_tracer, t->r.direction.x, t->r.direction.y, t->r.direction.z);
    matrix_rotate(matrix_tracer, 90.0F, 0.0F, 1.0F, 0.0F);
    matrix_upload(matrix_view, matrix_tracer);
    kv6_render(
        (kv6_t*[]) {
            &model_semi_tracer,
            &model_smg_tracer,
            &model_shotgun_tracer,
        }[t->type],
        TEAM_SPECTATOR);

    return false;
}

void tracer_render() {
    entitysys_iterate(&tracers, NULL, tracer_render_single);
}

static bool tracer_update_single(void* obj, void* user) {
    struct Tracer* t = (struct Tracer*)obj;
    float dt = *(float*)user;

    float len = distance3D(t->x, t->y, t->z, t->r.origin.x, t->r.origin.y, t->r.origin.z);

    // 128.0[m] / 256.0[m/s] = 0.5[s]
    if((t->hit.type != CAMERA_HITTYPE_NONE && len > pow(t->hit.distance, 2)) || window_time() - t->created > 0.5F) {
        if(t->hit.type != CAMERA_HITTYPE_NONE)
            sound_create(SOUND_WORLD, &sound_impact, t->r.origin.x, t->r.origin.y, t->r.origin.z);

        return true;
    } else {
        t->r.origin.x += t->r.direction.x * 32.0F * dt;
        t->r.origin.y += t->r.direction.y * 32.0F * dt;
        t->r.origin.z += t->r.direction.z * 32.0F * dt;
    }

    return false;
}

void tracer_update(float dt) {
    entitysys_iterate(&tracers, &dt, tracer_update_single);
}

void tracer_init() {
    entitysys_create(&tracers, sizeof(struct Tracer), PLAYERS_MAX);
}
