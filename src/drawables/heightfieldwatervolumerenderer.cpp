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

#include <heightfieldwatervolumerenderer.hpp>

namespace Orbis {

namespace Drawable {

void HeightFieldWaterVolumeRenderer::drawImplementation(osg::State& state) const
{
	if(waterVolume() == 0) {
		return;
	}
/*
	// working in a column-at-a-time basis
	for(unsigned i = 0; i < waterVolume()->size(); i++) {
		for(unsigned j = 0; j < waterVolume()->size(); j++) {
			for(unsigned k = 0; waterVolume()->status(i, j, k) != WaterVolume::BOUNDARY; k++) {
			}
		}
	}
*/
}

} } // namespace declarations
