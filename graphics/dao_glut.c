/*
// Dao Standard Modules
// http://www.daovm.net
//
// Copyright (c) 2012, Limin Fu
// All rights reserved.
// 
// Redistribution and use in source and binary forms, with or without modification,
// are permitted provided that the following conditions are met:
// 
// * Redistributions of source code must retain the above copyright notice,
//   this list of conditions and the following disclaimer.
// * Redistributions in binary form must reproduce the above copyright notice,
//   this list of conditions and the following disclaimer in the documentation
//   and/or other materials provided with the distribution.
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY
// EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
// OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
// SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT
// OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
// HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
// OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/


#if defined(__APPLE__)
#  include <OpenGL/gl.h>
#  include <GLUT/glut.h>
#else
#  include <GL/gl.h>
#  include <GL/glut.h>
#endif

#include <math.h>
#include <stdio.h>
#include "dao_opengl.h"


static float window_width = 300.0;
static float window_height = 200.0;
static float fps_limit = 10.0;

static int fps_count = 0;
static int current_time = 0;
static int last_update = 0;
static int count_reset = 0;
static int test_fps = 0;

DaoVmSpace *__daoVmSpace = NULL;

DaoxGraphicsScene *daox_current_scene = NULL;


DaoRoutine* Dao_Get_Object_Method( DaoCdata *cd, DaoObject **obj, const char *name )
{
  DaoRoutine *meth;
  if( cd == NULL ) return NULL;
  *obj = DaoCdata_GetObject( cd );
  if( *obj == NULL ) return NULL;
  return DaoObject_GetMethod( *obj, name );
}

void DaoxGraphics_CallMethod( DaoxGraphicsScene *scene, const char *method )
{
	DaoObject *obj = NULL;
	DaoRoutine *rout = Dao_Get_Object_Method( (DaoCdata*)scene, & obj, method );
	DaoProcess *proc;

	if( rout == NULL || obj == NULL ) return;
	proc = DaoVmSpace_AcquireProcess( __daoVmSpace );

	rout = DaoRoutine_Resolve( rout, (DaoValue*) obj, NULL, 0 );
	if( rout == NULL ) goto Finalize;
	DaoProcess_Call( proc, rout, (DaoValue*) obj, NULL, 0 );
Finalize:
	DaoVmSpace_ReleaseProcess( __daoVmSpace, proc );
}

int DaoxGraphics_CallKeyboardMethod( DaoxGraphicsScene *scene, const char *method, int key, int x, int y )
{
	DaoObject *obj = NULL;
	DaoRoutine *rout = Dao_Get_Object_Method( (DaoCdata*)scene, & obj, method );
	DaoProcess *proc;
	DaoValue **params;

	if( rout == NULL || obj == NULL ) return 0;
	proc = DaoVmSpace_AcquireProcess( __daoVmSpace );

	DaoProcess_NewInteger( proc, (daoint) key );
	DaoProcess_NewInteger( proc, (daoint) x );
	DaoProcess_NewInteger( proc, (daoint) y );
	params = DaoProcess_GetLastValues( proc, 3 );
	rout = DaoRoutine_Resolve( rout, (DaoValue*) obj, params, 3 );
	if( rout == NULL ) goto Finalize;
	DaoProcess_Call( proc, rout, (DaoValue*) obj, params, 3 );
Finalize:
	DaoVmSpace_ReleaseProcess( __daoVmSpace, proc );
	return rout != NULL;
}

void DaoxGraphics_UpdateScene( DaoxGraphicsScene *scene )
{
	DaoxGraphics_CallMethod( scene, "Update" );
}

void DaoxGraphics_glutIdle(void)
{
	if( daox_current_scene ) DaoxGraphics_CallMethod( daox_current_scene, "Idle" );
}
void DaoxGraphics_ApplyMaxFPS( int fps_limit )
{
	int interval;
	current_time = glutGet(GLUT_ELAPSED_TIME);
	interval = current_time - last_update;
	if( interval < 1000.0 / fps_limit ) usleep( 2000 * (1000.0 / fps_limit - interval) );
	last_update = current_time;
}
void DaoxGraphics_TestFPS()
{
	fps_count += 1;

	if( last_update ){
		printf( "FPS: %9.1f\n", 1000.0*fps_count / (float)(current_time - count_reset) );
		fflush( stdout );
	}
	if( (current_time - count_reset) > 5000 ){
		fps_count = (int)(1000.0 * fps_count / (float)(current_time - count_reset));
		count_reset = current_time - 1000;
	}
}
void DaoxGraphics_glutDisplay(void)
{
	if( daox_current_scene ){
		DaoxGraphics_UpdateScene( daox_current_scene );
		DaoxGraphics_glDrawScene( daox_current_scene, daox_current_scene->viewport );
	}

	glutSwapBuffers();

	DaoxGraphics_ApplyMaxFPS( fps_limit );

	if( test_fps ) DaoxGraphics_TestFPS();
}

void DaoxGraphics_glutReshape( int width, int height )
{
}

void DaoxGraphics_Zoom( int zoomin )
{
	DaoxBounds box = daox_current_scene->viewport;
	float width, height, dw, dh;
	width = box.right - box.left;
	height = box.top - box.bottom;
	dw = 0.0;
	dh = 0.0;
	if( zoomin ){
		dw = width / 6;
		dh = height / 6;
	}else{
		dw = - width / 4;
		dh = - height / 4;
	}
	box.left   += dw;
	box.right  -= dw;
	box.bottom += dh;
	box.top    -= dh;
	DaoxGraphicsScene_SetViewport( daox_current_scene, box.left, box.right, box.bottom, box.top );
}
void DaoxGraphics_glutKeyboard( unsigned char key, int x, int y )
{
	if( daox_current_scene == NULL ) return;
	if( DaoxGraphics_CallKeyboardMethod( daox_current_scene, "OnKeyboard", key, x, y ) ) return;

	if( key == '+' ){
		DaoxGraphics_Zoom( 1 );
	}else if( key == '-' ){
		DaoxGraphics_Zoom( 0 );
	}
}

void DaoxGraphics_glutSpecialKeyboard( int key, int x, int y )
{
	if( daox_current_scene == NULL ) return;
	DaoxGraphics_CallKeyboardMethod( daox_current_scene, "OnKeyboard", key, x, y );
}

enum ActionType
{
	ROTATION ,
	MOVING
};

static int last_x = 0;
static int last_y = 0;
static int action_type = ROTATION;

void DaoxGraphics_glutButton( int button, int state, int x, int y )
{
	last_x = x;
	last_y = y;
	action_type = button == GLUT_LEFT_BUTTON ? ROTATION : MOVING;
}

void DaoxGraphics_Rotate( int x, int y )
{
	DaoxTransform rotate = {1.0,0.0,0.0,1.0,0.0,0.0};
	DaoxBounds box = daox_current_scene->viewport;
	DaoxPoint start, end, center = {0.0,0.0};
	double W2 = 0.5 * window_width;
	double H2 = 0.5 * window_height;
	double area, cosine, sine;

	start.x = last_x - W2;
	start.y = last_y - H2;
	end.x = x - W2;
	end.y = y - H2;

	area = DaoxTriangle_Area( center, start, end );
	cosine = DaoxTriangle_AngleCosine( center, start, end );
	sine = sqrt( 1.0 - cosine * cosine );


	rotate.Axx = rotate.Ayy = cosine;
	if( area < 0.0 ){
		rotate.Axy = - sine;
		rotate.Ayx =   sine;
	}else{
		rotate.Axy =   sine;
		rotate.Ayx = - sine;
	}
	DaoxTransform_Multiply( & daox_current_scene->transform, rotate );

Done:
	last_x = x;
	last_y = y;
}
void DaoxGraphics_Move( int x, int y )
{
	DaoxBounds box = daox_current_scene->viewport;
	float xscale = (box.right - box.left) / window_width;
	float yscale = (box.top - box.bottom) / window_height;
	box.left   -= (x - last_x) * xscale;
	box.right  -= (x - last_x) * xscale;
	box.bottom += (y - last_y) * xscale;
	box.top    += (y - last_y) * xscale;
	last_x = x;
	last_y = y;
	if( box.left > 0.9*window_width ) return;
	if( box.right < 0.1*window_width ) return;
	if( box.bottom > 0.9*window_height ) return;
	if( box.top < 0.1*window_height ) return;
	DaoxGraphicsScene_SetViewport( daox_current_scene, box.left, box.right, box.bottom, box.top );
}
void DaoxGraphics_glutDrag( int x, int y )
{
	if( action_type == ROTATION ){
		DaoxGraphics_Rotate( x, y );
	}else{
		DaoxGraphics_Move( x, y );
	}
}

void DaoxGraphics_glutMove( int x, int y )
{
}

void DaoxGraphics_glutInit(int width, int height, const char *title)
{
  int i, argc = 1;
  char *argv = "Dao Graphics";

  glutInit( &argc, &argv );
  
  glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_STENCIL | GLUT_MULTISAMPLE);
  
  glutInitWindowPosition(0,0);
  glutInitWindowSize(width,height);
  glutCreateWindow(title);
  
  glutDisplayFunc(DaoxGraphics_glutDisplay);
  //glutIdleFunc(DaoxGraphics_glutIdle);
  glutIdleFunc(DaoxGraphics_glutDisplay);
  glutReshapeFunc(DaoxGraphics_glutReshape);
  glutKeyboardFunc(DaoxGraphics_glutKeyboard);
  glutSpecialFunc(DaoxGraphics_glutSpecialKeyboard);
  glutMouseFunc(DaoxGraphics_glutButton);
  glutMotionFunc(DaoxGraphics_glutDrag);
  glutPassiveMotionFunc(DaoxGraphics_glutMove);
  
  glDisable(GL_DEPTH_TEST);
  glEnable(GL_LINE_SMOOTH);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glHint(GL_LINE_SMOOTH_HINT, GL_DONT_CARE);
  glClearColor(1.0, 1.0, 1.0, 0.0);
}



static void GLUT_Init( DaoProcess *proc, DaoValue *p[], int N )
{
	char *title = DaoValue_TryGetMBString( p[2] );
	window_width = DaoValue_TryGetInteger( p[0] );
	window_height = DaoValue_TryGetInteger( p[1] );
	fps_limit = DaoValue_TryGetInteger( p[3] );
	test_fps = DaoValue_TryGetInteger( p[4] );
	DaoxGraphics_glutInit( window_width, window_height, title );
}
static void GLUT_SetScene( DaoProcess *proc, DaoValue *p[], int N )
{
	daox_current_scene = (DaoxGraphicsScene*) p[0];
	daox_current_scene->defaultWidth = window_width;
	daox_current_scene->defaultHeight = window_height;
}

static void GLUT_Show( DaoProcess *proc, DaoValue *p[], int N )
{
	GLUT_SetScene( proc, p, N );
	DaoxGraphicsScene_SetViewport( daox_current_scene,
			-window_width/2, window_width/2, -window_height/2, window_height/2 );
	glutMainLoop();
}
static DaoFuncItem DaoxGLUTMeths[]=
{
	{ GLUT_Init,      "glutInit( width = 300, height = 200, title = '', fps=10, test_fps=0 )" },
	{ GLUT_Show,      "glutShow( scene : GraphicsScene )" },
	{ GLUT_SetScene,  "glutSetScene( scene : GraphicsScene )" },
	{ NULL, NULL }
};


DAO_DLL int DaoGLUT_OnLoad( DaoVmSpace *vmSpace, DaoNamespace *ns )
{
	__daoVmSpace = vmSpace;
	DaoNamespace_WrapFunctions( ns, DaoxGLUTMeths );
	return 0;
}
