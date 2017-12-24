#include "common.h"

//for future opengl-es abstraction layer

void glx_enable_sphericalfog() {
    glEnable(GL_TEXTURE_2D);
    glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_DECAL);
    glBindTexture(GL_TEXTURE_2D,texture_gradient.texture_id);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP);
    glTexGeni(GL_T,GL_TEXTURE_GEN_MODE,GL_EYE_LINEAR);
    glTexGeni(GL_S,GL_TEXTURE_GEN_MODE,GL_EYE_LINEAR);
    float t_plane[4] = {1.0F/settings.render_distance/2.0F,0.0F,0.0F,-camera_x/settings.render_distance/2.0F+0.5F};
    float s_plane[4] = {0.0F,0.0F,1.0F/settings.render_distance/2.0F,-camera_z/settings.render_distance/2.0F+0.5F};
    glTexGenfv(GL_T,GL_EYE_PLANE,t_plane);
    glTexGenfv(GL_S,GL_EYE_PLANE,s_plane);
    glEnable(GL_TEXTURE_GEN_T);
    glEnable(GL_TEXTURE_GEN_S);
}

void glx_disable_sphericalfog() {
    glDisable(GL_TEXTURE_GEN_T);
    glDisable(GL_TEXTURE_GEN_S);
    glBindTexture(GL_TEXTURE_2D,0);
    glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
    glDisable(GL_TEXTURE_2D);
}
