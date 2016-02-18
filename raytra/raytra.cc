#include<cstdlib>
#include"hw1.h"

//#define IM_DEBUGGING

myCamera camera;
Array2D<Rgba> image;
int height, width;
vector<mySurface*> objectsList;
// this is called from the parseSceneFile function, which uses
// it to get the float from the correspoding position on the line.
//
// return the corresponding token in the inString. Errors out
// if you've asked for more than are in the line.
//
// you really don't need to know what is going on in here, I think.
//
float getTokenAsFloat (string inString, int whichToken)
{
    
    float thisFloatVal = 0.;    // the return value
    
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
void parseSceneFile (char *filnam)
{
    
    ifstream inFile(filnam);    // open the file
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
                
                float x, y, z, r;
                x = getTokenAsFloat (line, 1); 
                y = getTokenAsFloat (line, 2); 
                z = getTokenAsFloat (line, 3); 
                r = getTokenAsFloat (line, 4); 

				mySphere *sphere = new mySphere(x, y, z, r);
				objectsList.push_back(sphere);
                // build your sphere here from the parameters
                // i.e. you must call your sphere constructor and set its position
                // and radius from the above values. You must also put your new
                // sphere into the objects list (which can be global)
                // So something like;
                // mySphereClass *ms = new mySphereClass (x, y, z, r);   // make a new instance of your sphere class
                // ms->setMaterial (lastMaterialLoaded)
                // objectsList->push_back (ms);  // objectsList is a global std:vector<surface *> for example.
                
#ifdef IM_DEBUGGING
                // if we're debugging, show what we got:
                cout << "got a sphere with ";
                cout << "parameters: " << x << " " << y << " " << z << " " << r << endl;
#endif
                break;
            }
            case 't':   // triangle
                break;
                
            case 'p':   // plane
                break;
                
            //
            // camera:
            //
            case 'c': {// camera
                // one trick here: the cameras pixel count (width, height) are integers,
                // so cast them.
				
                float x, y, z, vx, vy, vz, d, iw, ih;
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
				camera = myCamera(x, y, z, vx, vy, vz, d, iw, ih, pw, ph);
				image.resizeErase(ph, pw);
				height = ph;
				width = pw;
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
                    case 'p':   // point light
                        break;
                    case 'd':   // directional light
                        break;
                    case 'a':   // ambient light
                        break;
                        
                }
                
                break;
            
            //
            // materials:
            //
            case 'm':   // material
                // the trick here: we should keep a pointer to the last material we read in,
                // so we can apply it to any subsequent geometry. Say it's called "lastMaterialLoaded"
                // we migh then do something like this:
                //
                //  1. read in the 10 material parameters: dr, dg, db, sr, sg, sb, r, ir, ig, ib
                //  2. call lastMaterialLoaded->setMaterial(dr, dg, db,...);
                //
                break;

            
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
}

void
writeRgba (const char fileName[],
           const Rgba *pixels,
           int width,
           int height)
{
    //
    // Write an RGBA image using class RgbaOutputFile.
    //
    //	- open the file
    //	- describe the memory layout of the pixels
    //	- store the pixels in the file
    //
    
    
    RgbaOutputFile file (fileName, width, height, WRITE_RGBA);
    file.setFrameBuffer (pixels, 1, width);
    file.writePixels (height);
}



void
readRgba (const char fileName[],
          Array2D<Rgba> &pixels,
          int &width,
          int &height)
{
    //
    // Read an RGBA image using class RgbaInputFile:
    //
    //	- open the file
    //	- allocate memory for the pixels
    //	- describe the memory layout of the pixels
    //	- read the pixels from the file
    //
    
    RgbaInputFile file (fileName);
    Box2i dw = file.dataWindow();
    
    width  = dw.max.x - dw.min.x + 1;
    height = dw.max.y - dw.min.y + 1;
    pixels.resizeErase (height, width);
    
    file.setFrameBuffer (&pixels[0][0] - dw.min.x - dw.min.y * width, 1, width);
    file.readPixels (dw.min.y, dw.max.y);
}

//
// the main just makes sure that there is an argument passed, which is
// supposed to be the scenefile.
//

void generateImage(Array2D<Rgba> &pixels,int &width, int &height, myCamera &camera) {
	for(int j = 0; j < height; j ++) {
		for(int i = 0; i < width; i ++) {
			float u = camera.l + (camera.r - camera.l) * (i + 0.5) / camera.nx;
			float v = camera.t - (camera.t - camera.b) * (j + 0.5) / camera.ny;
			myVector dir = -camera.d * camera.w + u * camera.u + v * camera.v;
			dir.normalize();
			myRay ray = myRay(camera.eye, dir);
			
#ifdef IM_DEBUGGING
                // if we're debugging, show what we got:
                cout << "(" << i <<"," << j <<"):";
                cout << "ray.point: " << ray.origin.x << " " << ray.origin.y << " " << ray.origin.z << ", " 
					 << "ray.dir: "<< ray.dir.x << " "  << ray.dir.y << " "  << ray.dir.z << endl;
#endif
				Rgba &px = pixels[j][i];
				px.r = 0;
				px.g = 0;
				px.b = 0;
				px.a = 1;
				for(vector<mySurface*>::iterator it = objectsList.begin(); it != objectsList.end(); ++it) {
					if ((*it)->intersect(ray)) {
						px.r = HALF_MAX;
						break;
					}

				}
		}
     }
}
int main (int argc, char *argv[])
{ 

    if (argc != 2) {
        cerr << "useage: raytra scenefilename" << endl;
        return -1;
    }
    
    parseSceneFile (argv[1]);

	generateImage(image, width, height, camera);
    writeRgba ("hw1.1.exr", &image[0][0], width, height);

    return 0;
}
