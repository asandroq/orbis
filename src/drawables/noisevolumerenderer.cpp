/*
 * The Orbis world simulator
 * Copyright (C) 2001-2004 Alex Sandro Queiroz e Silva
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * The author may be contacted by eletronic e-mail at <asandro@lcg.dc.ufc.br>
 */

#ifdef __GNUG__
#pragma implementation
#endif

#include <iostream>

#include <osg/BlendFunc>
#include <osg/Texture3D>

#include <noisevolumerenderer.hpp>

namespace {

/*
 * Improved Noise implementation taken from http://mrl.nyu.edu/~perlin/noise/
 * Note: Perlin likes Bossa-Nova :-)
 */
class ImprovedNoise {
public:
	static double noise(double x, double y, double z) {
		if(!_init) {
			init();
			_init = true;
		}

		// find unit cube that contains point.
		int X = static_cast<int>(floor(x)) & 255;
		int Y = static_cast<int>(floor(y)) & 255;
		int Z = static_cast<int>(floor(z)) & 255;

		// find relative x,y,z of point in cube.
		x -= floor(x);
		y -= floor(y);
		z -= floor(z);

		// compute fade curves for each of x,y,z.
		double u = fade(x);
		double v = fade(y);
		double w = fade(z);

		// hash coordinates of the 8 cube corners,
		int A  = p[X  ]+Y;
		int AA = p[A  ]+Z;
		int AB = p[A+1]+Z;
		int B  = p[X+1]+Y;
		int BA = p[  B]+Z;
		int BB = p[B+1]+Z;      

		// and add blended results from  8 corners of cube
		return lerp(w, lerp(v, lerp(u, grad(p[AA  ], x  , y  , z  ),  
                                       grad(p[BA  ], x-1, y  , z  )), 
                               lerp(u, grad(p[AB  ], x  , y-1, z  ),  
                                       grad(p[BB  ], x-1, y-1, z  ))),
                       lerp(v, lerp(u, grad(p[AA+1], x  , y  , z-1),  
                                       grad(p[BA+1], x-1, y  , z-1)), 
                               lerp(u, grad(p[AB+1], x  , y-1, z-1),
                                       grad(p[BB+1], x-1, y-1, z-1))));
	}

private:
	static void init() {
		for(int i = 0; i < 256; i++) {
			p[256+i] = p[i] = permutation[i];
		}
	}

	static double fade(double t) {
		return t * t * t * (t * (t * 6.0 - 15.0) + 10.0);
	}

	static double lerp(double t, double a, double b) {
		return a + t * (b - a);
	}

	static double grad(int hash, double x, double y, double z) {
		// convert lo 4 bits of hash code
		int h = hash & 15;

		// into 12 gradient directions.
		double u = h < 8 ? x : y;
		double v = h < 4 ? y : h == 12 || h == 14 ? x : z;

		return ((h&1) == 0 ? u : -u) + ((h&2) == 0 ? v : -v);
	}

	static bool _init;
	static int p[];
	static int permutation[];
};

bool ImprovedNoise::_init = false;

int ImprovedNoise::p[512];

int ImprovedNoise::permutation[] =  {
	151,160,137,91,90,15,131,13,201,95,96,53,194,233,7,225,140,36,103,30,69,
	142,8,99,37,240,21,10,23,190, 6,148,247,120,234,75,0,26,197,62,94,252,219,
	203,117,35,11,32,57,177,33,88,237,149,56,87,174,20,125,136,171,168, 68,175,
	74,165,71,134,139,48,27,166,77,146,158,231,83,111,229,122,60,211,133,230,
	220,105,92,41,55,46,245,40,244,102,143,54, 65,25,63,161, 1,216,80,73,209,
	76,132,187,208, 89,18,169,200,196, 135,130,116,188,159,86,164,100,109,198,
	173,186, 3,64,52,217,226,250,124,123, 5,202,38,147,118,126,255,82,85,212,
	207,206,59,227,47,16,58,17,182,189,28,42,223,183,170,213,119,248,152, 2,44,
	154,163, 70,221,153,101,155,167, 43,172,9,129,22,39,253, 19,98,108,110,79,
	113,224,232,178,185, 112,104,218,246,97,228,251,34,242,193,238,210,144,12,
	191,179,162,241, 81,51,145,235,249,14,239,107,49,192,214, 31,181,199,106,
	157,184, 84,204,176,115,121,50,45,127, 4,150,254,138,236,205,93,222,114,67,
	29,24,72,243,141,128,195,78,66,215,61,156,180
};

} // anonymous namespace

namespace Orbis {

namespace Drawable {

NoiseVolumeRenderer::NoiseVolumeRenderer(const WaterVolume* const wv, double threshold)
	: WaterVolumeRenderer(wv, threshold)
{
	osg::StateSet *stateSet = getOrCreateStateSet();

	// activating blending in this drawable
	stateSet->setMode(GL_BLEND, osg::StateAttribute::ON);
	stateSet->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
	stateSet->setMode(GL_DEPTH_TEST, osg::StateAttribute::OFF);
	stateSet->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);
	osg::BlendFunc *bf = new osg::BlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	stateSet->setAttribute(bf);
/*
	const unsigned sx = 64;
	const unsigned sy = 64;
	const unsigned sz = 64;

	// 3D image to hold noise
	osg::Image *img = new osg::Image;
	img->allocateImage(sx, sy, sz, GL_RGB, GL_FLOAT);
	float *buf = reinterpret_cast<float*>(img->data());
	for(unsigned i = 0; i < sx; i++) {
		for(unsigned j = 0; j < sy; j++) {
			for(unsigned k = 0; k < sz; k++) {
				double n = ImprovedNoise::noise(i / static_cast<double>(sx),
												j / static_cast<double>(sy),
												k / static_cast<double>(sz)) + 0.5;
				buf[0] = n;
				buf[1] = n;
				buf[2] = n;

				buf += 3;
			}
		}
	}

	// setting up 3D smoke texture
	osg::Texture3D *tex = new osg::Texture3D;
	tex->setImage(img);
	stateSet->setTextureAttributeAndModes(0, tex, osg::StateAttribute::ON);
*/
	setUseDisplayList(false);
}

void NoiseVolumeRenderer::drawImplementation(osg::State& state) const
{
	if(waterVolume() == 0) {
		return;
	}

	Point p;
	Point o = waterVolume()->origin();
	double dx = 1.0 / ((waterVolume()->sizeX() - 1) * waterVolume()->stepX());
	double dy = 1.0 / ((waterVolume()->sizeY() - 1) * waterVolume()->stepY());
	double dz = 1.0 / ((waterVolume()->sizeZ() - 1) * waterVolume()->stepZ());

	glBegin(GL_LINES);
		// x axis
		glColor4f(1.0, 0.0, 0.0, 1.0);
		p = waterVolume()->point(0, 0, 0);
		glVertex3d(p.x(), p.y(), p.z());
		p = waterVolume()->point(waterVolume()->sizeX()-1, 0, 0);
		glVertex3d(p.x(), p.y(), p.z());
		// y axis
		glColor4f(0.0, 1.0, 0.0, 1.0);
		p = waterVolume()->point(0, 0, 0);
		glVertex3d(p.x(), p.y(), p.z());
		p = waterVolume()->point(0, waterVolume()->sizeY()-1, 0);
		glVertex3d(p.x(), p.y(), p.z());
		// z axis
		glColor4f(0.0, 0.0, 1.0, 1.0);
		p = waterVolume()->point(0, 0, 0);
		glVertex3d(p.x(), p.y(), p.z());
		p = waterVolume()->point(0, 0, waterVolume()->sizeZ()-1);
		glVertex3d(p.x(), p.y(), p.z());
	glEnd();

	Locker lock(waterVolume());

	// showing velocities
/*	glBegin(GL_LINES);
	glColor4d(1.0, 1.0, 1.0, 1.0);
	for(unsigned i = 0; i < waterVolume()->sizeX(); i++) {
		for(unsigned j = 0; j < waterVolume()->sizeY(); j++) {
			for(unsigned k = 0; k < waterVolume()->sizeZ(); k++) {
				Vector v = waterVolume()->velocity(i, j, k);
				Point p1 = waterVolume()->point(i, j, k);
				Point p2 = p1 + v;
				glVertex3d(p1.x(), p1.y(), p1.z());
				glVertex3d(p2.x(), p2.y(), p2.z());
			}
		}
	}
	glEnd();*/

	double tr = 1.0 / threshold();
	// showing densities
	for(unsigned i = 1; i < waterVolume()->sizeX() - 1; i++) {
		for(unsigned j = 1; j < waterVolume()->sizeY() - 1; j++) {
			for(unsigned k = 1; k < waterVolume()->sizeZ() - 1; k++) {
				Vector n;
				double x, y, z, t, d;

				// cube vertices
				Point p1 = waterVolume()->point(  i,   j,   k);
				Point p2 = waterVolume()->point(i+1,   j, k+1);
				Point p3 = waterVolume()->point(i+1,   j,   k);
				Point p4 = waterVolume()->point(i+1, j+1, k+1);
				Point p5 = waterVolume()->point(i+1, j+1,   k);
				Point p6 = waterVolume()->point(  i, j+1, k+1);
				Point p7 = waterVolume()->point(  i, j+1,   k);
				Point p8 = waterVolume()->point(  i,   j, k+1);

				glBegin(GL_TRIANGLE_STRIP);

				// first face
				n = ((p3 - p1) ^ (p2 - p1)).normalise();
				glNormal3d(n.x(), n.y(), n.z());

				x = (p8.x() - o.x()) * dx;
				y = (p8.y() - o.y()) * dy;
				z = (p8.z() - o.z()) * dz;
				t = ImprovedNoise::noise(x, y, z) + 0.5;
				d = waterVolume()->density(i, j, k+1) * tr;
				glColor4d(t, t, t, d);
				glTexCoord3d(x, y, z);
				glVertex3d(p8.x(), p8.y(), p8.z());

				x = (p1.x() - o.x()) * dx;
				y = (p1.y() - o.y()) * dy;
				z = (p1.z() - o.z()) * dz;
				t = ImprovedNoise::noise(x, y, z) + 0.5;
				d = waterVolume()->density(i, j, k) * tr;
				glColor4d(t, t, t, d);
				glTexCoord3d(x, y, z);
				glVertex3d(p1.x(), p1.y(), p1.z());

				x = (p2.x() - o.x()) * dx;
				y = (p2.y() - o.y()) * dy;
				z = (p2.z() - o.z()) * dz;
				t = ImprovedNoise::noise(x, y, z) + 0.5;
				d = waterVolume()->density(i+1, j, k+1) * tr;
				glColor4d(t, t, t, d);
				glTexCoord3d(x, y, z);
				glVertex3d(p2.x(), p2.y(), p2.z());

				x = (p3.x() - o.x()) * dx;
				y = (p3.y() - o.y()) * dy;
				z = (p3.z() - o.z()) * dz;
				t = ImprovedNoise::noise(x, y, z) + 0.5;
				d = waterVolume()->density(i+1, j, k) * tr;
				glColor4d(t, t, t, d);
				glTexCoord3d(x, y, z);
				glVertex3d(p3.x(), p3.y(), p3.z());

				// second face
				n = ((p5 - p3) ^ (p4 - p3)).normalise();
				glNormal3d(n.x(), n.y(), n.z());

				x = (p4.x() - o.x()) * dx;
				y = (p4.y() - o.y()) * dy;
				z = (p4.z() - o.z()) * dz;
				t = ImprovedNoise::noise(x, y, z) + 0.5;
				d = waterVolume()->density(i+1, j+1, k+1) * tr;
				glColor4d(t, t, t, d);
				glTexCoord3d(x, y, z);
				glVertex3d(p4.x(), p4.y(), p4.z());

				x = (p5.x() - o.x()) * dx;
				y = (p5.y() - o.y()) * dy;
				z = (p5.z() - o.z()) * dz;
				t = ImprovedNoise::noise(x, y, z) + 0.5;
				d = waterVolume()->density(i+1, j+1, k) * tr;
				glColor4d(t, t, t, d);
				glTexCoord3d(x, y, z);
				glVertex3d(p5.x(), p5.y(), p5.z());

				// third face
				n = ((p7 - p5) ^ (p6 - p5)).normalise();
				glNormal3d(n.x(), n.y(), n.z());

				x = (p6.x() - o.x()) * dx;
				y = (p6.y() - o.y()) * dy;
				z = (p6.z() - o.z()) * dz;
				t = ImprovedNoise::noise(x, y, z) + 0.5;
				d = waterVolume()->density(i, j+1, k+1) * tr;
				glColor4d(t, t, t, d);
				glTexCoord3d(x, y, z);
				glVertex3d(p6.x(), p6.y(), p6.z());

				x = (p7.x() - o.x()) * dx;
				y = (p7.y() - o.y()) * dy;
				z = (p7.z() - o.z()) * dz;
				t = ImprovedNoise::noise(x, y, z) + 0.5;
				d = waterVolume()->density(i, j+1, k) * tr;
				glColor4d(t, t, t, d);
				glTexCoord3d(x, y, z);
				glVertex3d(p7.x(), p7.y(), p7.z());

				// fourth face
				n = ((p1 - p7) ^ (p8 - p7)).normalise();
				glNormal3d(n.x(), n.y(), n.z());

				x = (p8.x() - o.x()) * dx;
				y = (p8.y() - o.y()) * dy;
				z = (p8.z() - o.z()) * dz;
				t = ImprovedNoise::noise(x, y, z) + 0.5;
				d = waterVolume()->density(i, j, k+1) * tr;
				glColor4d(t, t, t, d);
				glTexCoord3d(x, y, z);
				glVertex3d(p8.x(), p8.y(), p8.z());

				x = (p1.x() - o.x()) * dx;
				y = (p1.y() - o.y()) * dy;
				z = (p1.z() - o.z()) * dz;
				t = ImprovedNoise::noise(x, y, z) + 0.5;
				d = waterVolume()->density(i, j, k) * tr;
				glColor4d(t, t, t, d);
				glTexCoord3d(x, y, z);
				glVertex3d(p1.x(), p1.y(), p1.z());

				glEnd();
			}
		}
	}
}

} } // namespace declarations
