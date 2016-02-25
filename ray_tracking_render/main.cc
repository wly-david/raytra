#include<cstdlib>
#include"hw1.h"
#include<cassert>

//#define IM_DEBUGGING

// this is called from the parseSceneFile function, which uses
// it to get the float from the correspoding position on the line.
//
// return the corresponding token in the inString. Errors out
// if you've asked for more than are in the line.
//
// you really don't need to know what is going on in here, I think.
//
double getTokenAsFloat (string inString, int whichToken)
{
    
    double thisFloatVal = 0.;    // the return value
    
    if (whichToken == 0) {
        cerr << "error: the first token on a line is a character!" << endl;
        exit (-1);
    }
    
    // c++ string class has no super-easy way to tokenize, let's use c's:
    char *cstr = new char [inString.size () + 1];
    
    strcpy (cstr, inString.c_str());
    
    char *p = strtok (cstr, " ");
    if (p == 0) {
        cerr << "error: the line has nothing on it!" << endl;
        exit (-1);
    }
    
    for (int i = 0; i < whichToken; i++) {
        p = strtok (0, " ");
        if (p == 0) {
            cerr << "error: the line is not long enough for your token request!" << endl;
            exit (-1);
        }
    }
    
    thisFloatVal = atof (p);
    
    delete[] cstr;
    
    return thisFloatVal;
}


//
// read the scene file.
//
// You'll need a few globals (or add arguments to this function): for the
// list of surfaces (geometric objects like spheres, triangles, planes) and
// another for the lights. These can be of type std::vector. You'll also
// need a global (or other parameter) for the camera.
//
// This should be pretty self-explanatory: it reads through the lines, and
// using the first character figures out what kind of command it is. It
// then calls the "getTokenAsFloat" routine to pull out the needed values.
// NOTE: since different commands take different number of arguments, you
// must take care not to call getTokenAsFloat for an index that is beyond the
// end of the line!
//
// One tricky bit: when a material is read in, we want
// to keep a pointer to it so that for the next geometric object read in ,
// we can add that material to the object. In the code that follows, I use the
// variable "lastSurfaceLoaded" to do that, but the code is commented out since
// I don't know the class names you will be using.
//
// Very minimal error check here. You might improve it slightly, but we'll
// only use "correct" scene files.
//
//
void parseSceneFile (char *filname, myCamera & camera, vector< mySurface * > &Surfaces, vector< myMaterial * > &Materials, vector< myLight * > &Lights)
{
    
    ifstream inFile(filname);    // open the file
    string line;
    
    if (! inFile.is_open ()) {  // if it's not open, error out.
        cerr << "can't open scene file" << endl;
        exit (-1);
    }
    
    
    // Note: you'll have to keep track of whatever the last material
    // you loaded in was, so you can apply it to any geometry that gets loaded. 
    // So here, you'll have something like:
    //
    // myMaterialClass *lastMaterialLoaded = 0;  // 0 or maybe a default material?
    //
    // and each time you load in a new piece of geometry (sphere, triangle, plane)
    // you will set its material to lastMaterialLoaded.

    int num_cams = 0;
	myMaterial *lastMaterialLoaded = NULL;

    while (! inFile.eof ()) {   // go through every line in the file until finished
        
        getline (inFile, line); // get the line
        
        switch (line[0])  {     // we'll decide which command based on the first character
            
            //
            // geometry types:
            //
            // NOTE: whichever type of geo you load in, set its material to
            // be "lastMaterialLoaded"
            //
            case 's': {
                // it's a sphere, load in the parameters
                
                double x, y, z, r;
                x = getTokenAsFloat (line, 1); 
                y = getTokenAsFloat (line, 2); 
                z = getTokenAsFloat (line, 3); 
                r = getTokenAsFloat (line, 4); 
				
                // build your sphere here from the parameters
                // i.e. you must call your sphere constructor and set its position
                // and radius from the above values. You must also put your new
                // sphere into the objects list (which can be global)
                // So something like;
                // mySphereClass *ms = new mySphereClass (x, y, z, r);   // make a new instance of your sphere class
                // ms->setMaterial (lastMaterialLoaded)
                // objectsList->push_back (ms);  // objectsList is a global std:vector<surface *> for example.

				mySphere *sphere = new mySphere(myPoint(x, y, z), r);
				assert(lastMaterialLoaded != NULL);
				sphere->setMaterial(lastMaterialLoaded);
				Surfaces.push_back(sphere);
                
#ifdef IM_DEBUGGING
                // if we're debugging, show what we got:
                cout << "got a sphere with ";
                cout << "parameters: " << x << " " << y << " " << z << " " << r << endl;
#endif
                break;
            }
            case 't': {// triangle
                double a1, b1, c1, a2, b2, c2, a3, b3, c3;
                a1 = getTokenAsFloat (line, 1);
                b1 = getTokenAsFloat (line, 2);
                c1 = getTokenAsFloat (line, 3);
                a2 = getTokenAsFloat (line, 4);
                b2 = getTokenAsFloat (line, 5);
                c2 = getTokenAsFloat (line, 6);
                a3 = getTokenAsFloat (line, 7);
                b3 = getTokenAsFloat (line, 8);
                c3 = getTokenAsFloat (line, 9);
				myTriangle *triangle = new myTriangle(myPoint(a1, b1, c1), myPoint(a2, b2, c2), myPoint(a3, b3, c3));
				assert(lastMaterialLoaded != NULL);
				triangle->setMaterial(lastMaterialLoaded);
				Surfaces.push_back(triangle);
                break;
			}
            case 'p': {  // plane
				double nx, ny, nz, d;
                nx = getTokenAsFloat (line, 1);
                ny = getTokenAsFloat (line, 2);
                nz = getTokenAsFloat (line, 3);
                d = getTokenAsFloat (line, 4);
				myPlane *plane = new myPlane(myVector(nx, ny, nz), d);
				assert(lastMaterialLoaded != NULL);
				plane->setMaterial(lastMaterialLoaded);
				Surfaces.push_back(plane);
                break;
			}
            //
            // camera:
            //
            case 'c': {// camera
                // one trick here: the cameras pixel count (width, height) are integers,
                // so cast them.
				
				++num_cams; // keep track of how many we read in

                double x, y, z, vx, vy, vz, d, iw, ih;
				int pw, ph;
                x = getTokenAsFloat (line, 1);
                y = getTokenAsFloat (line, 2);
                z = getTokenAsFloat (line, 3);
                vx = getTokenAsFloat (line, 4);
                vy = getTokenAsFloat (line, 5);
                vz = getTokenAsFloat (line, 6);
                d = getTokenAsFloat (line, 7);
                iw = getTokenAsFloat (line, 8);
                ih = getTokenAsFloat (line, 9);
                pw = (int)getTokenAsFloat (line, 10);
                ph = (int)getTokenAsFloat (line, 11);
				camera.init(myPoint(x,y,z), myVector(vx, vy, vz), d, iw, ih, pw, ph);
#ifdef IM_DEBUGGING
                // if we're debugging, show what we got:
                cout << "got a camera with ";
                cout << "parameters: " << x << " " << y << " " << z << " " 
					<< vx << " "  << vy << " "  << vz << " "  << d << " "  << iw << " "  << ih << " "  << pw << " "  << ph << " " << endl;
#endif
                break;
			}
            //
            // lights:
            //
            case 'l':   // light
                
                // slightly different from the rest, we need to examine the second param,
                // which is at the third position on the line:
                switch (line[2]) {
                    case 'p': {  // point light
						double x, y, z, r, g, b;
						x = getTokenAsFloat (line, 2);
						y = getTokenAsFloat (line, 3);
						z = getTokenAsFloat (line, 4);
						r = getTokenAsFloat (line, 5);
						g = getTokenAsFloat (line, 6);
						b = getTokenAsFloat (line, 7);
						p_light *pointLight = new p_light(myPoint(x,y,z), r, g, b);
						Lights.push_back(pointLight);

                        break;
					}
                    case 'd':   // directional light
                        break;
                    case 'a':   // ambient light
                        break;
                        
                }
                
                break;
            
            //
            // materials:
            //
            case 'm':  { // material
                // the trick here: we should keep a pointer to the last material we read in,
                // so we can apply it to any subsequent geometry. Say it's called "lastMaterialLoaded"
                // we migh then do something like this:
                //
                //  1. read in the 10 material parameters: dr, dg, db, sr, sg, sb, r, ir, ig, ib
                //  2. call lastMaterialLoaded->setMaterial(dr, dg, db,...);
                //
                double dr, dg, db, sr, sg, sb, r, ir, ig, ib;
                dr = getTokenAsFloat (line, 1);
                dg = getTokenAsFloat (line, 2);
                db = getTokenAsFloat (line, 3);
                sr = getTokenAsFloat (line, 4);
                sg = getTokenAsFloat (line, 5);
                sb = getTokenAsFloat (line, 6);
                r = getTokenAsFloat (line, 7);
                ir = getTokenAsFloat (line, 8);
                ig = getTokenAsFloat (line, 9);
                ib = getTokenAsFloat (line, 10);
				lastMaterialLoaded = new myMaterial(myVector(dr,dg,db), myVector(sr,sg,sb), myVector(ir,ig,ib), r);
				Materials.push_back(lastMaterialLoaded);
                break;
			}
            
            case '/':
                // don't do anything, it's a comment
                break;
                

            //
            // options
            //
            case 'o':   // make your own options if you wish
                break;
        }
        
    }
	
    // make sure we read in 1 camera, no more no less.
    assert (num_cams >= 1); // make sure there are some surfaces
    if (num_cams != 1) {
        std::cerr << "scene file error: exactly ONE camera must be defined." << endl;
    }
}

//
// the main just makes sure that there is an argument passed, which is
// supposed to be the scenefile.
//

int main (int argc, char *argv[])
{ 
    if (argc != 3) {
        // error condition: 
        std::cout << "usage: raytra scenefile outputimage" << std::endl;
    }
    
	myCamera camera;
	
	vector<mySurface*> Surfaces;
    vector<myMaterial*> Materials;
	vector<myLight*> Lights;
    parseSceneFile (argv[1], camera, Surfaces, Materials, Lights);
	
    assert (Materials.size () != 0); // make sure there are some materials
    assert (Surfaces.size () != 0); // make sure there are some surfaces
    assert (Lights.size () != 0); // make sure there are some lights
	
	camera.renderScene(Surfaces, Lights);
	camera.writeImage(argv[2]);
	
	for(std::vector<mySurface*>::iterator it = Surfaces.begin(); it != Surfaces.end(); ++it) {
		delete (*it);
	}
	for(std::vector<myLight*>::iterator it = Lights.begin(); it != Lights.end(); ++it) {
		delete (*it);
	}
	for(std::vector<myMaterial*>::iterator it = Materials.begin(); it != Materials.end(); ++it) {
		delete (*it);
	}
    return 0;
}
