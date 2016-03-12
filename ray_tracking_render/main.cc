#include<cstdlib>
#include"hw1.h"
#include<cassert>

//#define IM_DEBUGGING

// Given the name of a wavefront (OBJ) file that consists JUST of
// vertices, triangles, and comments, read it into the tris and verts
// vectors.
//
// tris is a vector of ints that is 3*n long, where n is the number of
// triangles. The ith triangle has vertex indexes 3*i, 3*i+1, and 3*i+2.
//
// The ith triangle has vertices:
//
//     verts[3*tris[3*i]], verts[3*tris[3*i]+1], verts[3*tris[3*i]+2],
//     verts[3*tris[3*i+1]], verts[3*tris[3*i+1]+1], verts[3*tris[3*i+1]+2],
//     verts[3*tris[3*i+2]], verts[3*tris[3*i+2]+1], verts[3*tris[3*i+2]+2]
//
// given in counterclockwise order with respect to the surface normal
//
// If you are using the supplied Parser class, you should probably make this
// a method on it: Parser::read_wavefront_file().
//
void read_wavefront_file (
    const string file,
    vector< int > &tris,
    vector< myPoint > &verts)
{
    // clear out the tris and verts vectors:
    tris.clear ();
    verts.clear ();
    ifstream in(file);
    if (! in.is_open ()) {  // if it's not open, error out.
        cerr << "can't open obj file" << endl;
		return;
    }
    char buffer[1025];
    string cmd;
    for (int line=1; in.good(); line++) {
        in.getline(buffer,1024);
        buffer[in.gcount()]=0;
        cmd="";
        istringstream iss (buffer);
        iss >> cmd;
        if (cmd[0]=='#' || cmd.empty()) {
            // ignore comments or blank lines
            continue;
        } 
        else if (cmd=="v") {
            // got a vertex:
            // read in the parameters:
            double pa, pb, pc;
            iss >> pa >> pb >> pc;
            verts.push_back (myPoint(pa, pb, pc));
         } 
        else if (cmd=="f") {
            // got a face (triangle)
            // read in the parameters:
            int i, j, k;
            iss >> i >> j >> k;
            // vertex numbers in OBJ files start with 1, but in C++ array
            // indices start with 0, so we're shifting everything down by
            // 1
            tris.push_back (i-1);
            tris.push_back (j-1);
            tris.push_back (k-1);
        } 
        else {
            cerr << "Parser error: invalid command at line " << line << endl;
        }
     }
    in.close();
    
 //   cout << "found this many tris, verts: " << tris.size () / 3.0 << "  " << verts.size () / 3.0 << endl;
}




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
void parseSceneFile (char *filname, myCamera & camera, vector<BVH_Node*> &BBoxes, vector< mySurface * > &Planes, vector< myMaterial * > &Materials, vector< myLight * > &Lights, ALight * &ambient)
{
    
    ifstream inFile(filname);    // open the file
    string line; 
    if (! inFile.is_open ()) {  // if it's not open, error out.
        cerr << "can't open scene file" << endl;
        exit (-1);
    }
    int num_cams = 0;
	myMaterial *lastMaterialLoaded = NULL;
	
    vector< int > tris;
    vector< myPoint > verts;
    while (! inFile.eof ()) {   // go through every line in the file until finished
        getline (inFile, line); // get the line
		if(!line.empty() && *line.rbegin() == '\r') {
			line.erase( line.length()-1, 1);
		}
        switch (line[0])  {     // we'll decide which command based on the first character
            // geometry types:
            //
            // NOTE: whichever type of geo you load in, set its material to
            // be "lastMaterialLoaded"
            case 's': {
                // it's a sphere, load in the parameters
                double x, y, z, r;
                x = getTokenAsFloat (line, 1); 
                y = getTokenAsFloat (line, 2); 
                z = getTokenAsFloat (line, 3); 
                r = getTokenAsFloat (line, 4); 				
				mySphere *sphere = new mySphere(myPoint(x, y, z), r);
				assert(lastMaterialLoaded != NULL);
				sphere->setMaterial(lastMaterialLoaded);
				BVH_Node *object = new BVH_Node(sphere);
				myBBox *bbox = sphere->generateBBox();
				BVH_Node *leaf = new BVH_Node(bbox);
				leaf->setLeft(object);
				BBoxes.push_back(leaf);
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
				BVH_Node *object = new BVH_Node(triangle);
				myBBox *bbox = triangle->generateBBox();
				BVH_Node *leaf = new BVH_Node(bbox);
				leaf->setLeft(object);
				BBoxes.push_back(leaf);
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
				Planes.push_back(plane);
                break;
			}
            // camera:
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
            // lights:
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
                    case 'a':   {// ambient light
						if (ambient != NULL) {
							cerr << "scene file error: at most ONE ambient light need to be defined." << endl;
						}
						double r, g, b;
						r = getTokenAsFloat (line, 2);
						g = getTokenAsFloat (line, 3);
						b = getTokenAsFloat (line, 4);
						ambient = new ALight(r, g, b);
                        break;
					}
                }
                break;
            // materials:
            case 'm':  { // material
                // the trick here: we should keep a pointer to the last material we read in,
                // so we can apply it to any subsequent geometry. Say it's called "lastMaterialLoaded"
                // we migh then do something like this:
                //  1. read in the 10 material parameters: dr, dg, db, sr, sg, sb, r, ir, ig, ib
                //  2. call lastMaterialLoaded->setMaterial(dr, dg, db,...);
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
			case 'w': {
				int pos = line.find_last_of(".");
				if(line.substr(pos + 1).compare("obj") != 0) {
					std::cerr << "Wrong obj filename!" << std::endl;
					break;
				} 
				const string filename = line.substr(2, pos + 4);
				read_wavefront_file (filename, tris, verts);
#ifdef IM_DEBUGGING
				cout <<"tris num: " << tris.size() / 3 << endl;
				cout <<"verts num: " << verts.size() << endl;
#endif
				int count = 0;
				myPoint triPoints[3];
				for(vector<int>::iterator it = tris.begin(); it != tris.end(); ++it) {
					triPoints[count] = verts[*it];
					count = (count + 1) % 3;
					if (count == 0) {						
						myTriangle *triangle = new myTriangle(triPoints[0], triPoints[1], triPoints[2]);
						assert(lastMaterialLoaded != NULL);
						triangle->setMaterial(lastMaterialLoaded);
						BVH_Node *object = new BVH_Node(triangle);
						myBBox *bbox = triangle->generateBBox();
						BVH_Node *leaf = new BVH_Node(bbox);
						leaf->setLeft(object);
						BBoxes.push_back(leaf);
					}
				}
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
	tris.clear();
	verts.clear();
    assert (num_cams >= 1); // make sure there are some surfaces
    if (num_cams != 1) {
        cerr << "scene file error: exactly ONE camera must be defined." << endl;
    }
}

//
// the main just makes sure that there is an argument passed, which is
// supposed to be the scenefile.
//

int main (int argc, char *argv[])
{ 
	int render_model = 3;
    if (argc != 3 && argc != 4) {
        // error condition: 
        cout << "usage: raytra scenefile outputimage" << endl;
		return -1;
    }
	else if (argc == 4) {
		render_model = atoi(argv[3]);
	}
	myCamera camera;
	camera.setModel(render_model);
	ALight* ambient = NULL;
	vector<BVH_Node*> BBoxes;
	vector<mySurface*> Planes;
    vector<myMaterial*> Materials;
	vector<myLight*> Lights;
    parseSceneFile (argv[1], camera, BBoxes, Planes, Materials, Lights, ambient);
    //assert (Materials.size () != 0); // make sure there are some materials
    //assert (Planes.size () != 0); // make sure there are some surfaces
    //assert (BBoxes.size () != 0); // make sure there are some BBoxes
    //assert (Lights.size () != 0); // make sure there are some lights
	BVH_Node * root = createTree(BBoxes, 0, BBoxes.size(), 0);

	camera.renderScene(root, BBoxes, Planes, Lights, ambient);
	camera.writeImage(argv[2]);
	
	for(vector<mySurface*>::iterator it = Planes.begin(); it != Planes.end(); ++it) {
		delete (*it);
	}
	for(vector<myLight*>::iterator it = Lights.begin(); it != Lights.end(); ++it) {
		delete (*it);
	}
	for(vector<myMaterial*>::iterator it = Materials.begin(); it != Materials.end(); ++it) {
		delete (*it);
	}
	removeTree(root);
	if (ambient != NULL)
		delete ambient;
    return 0;
}
