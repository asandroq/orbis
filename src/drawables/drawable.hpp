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
 *
 * $Id: drawable.hpp,v 1.3 2004/05/04 15:51:22 asandro Exp $
 */

#ifndef __ORBIS_DRAWABLE_HPP__
#define __ORBIS_DRAWABLE_HPP__

#ifdef __GNUG__
#pragma interface
#endif

#include <osg/Geometry>

namespace Orbis {

namespace Drawable {

/*!
 * \brief This is the base class of all objects that can be drawn.
 *
 * The Drawable class already has provisions for some texture mapping.
 */
class Drawable : public osg::Geometry {
public:
	/*!
	 * \brief Constructor.
	 */
	Drawable();

	/*!
	 * \brief Copy constructor.
	 */
	Drawable(const Drawable& src, const osg::CopyOp& copyOp);

	/*!
	 * \brief Gets the texture.
	 * \return The name of the file containing the texture.
	 */
	std::string texture() const;

	/*!
	 * \brief Sets the texture.
	 * \param texture The name of the file containing the texture.
	 */
	void setTexture(const std::string& texture);

protected:
	/*!
	 * \brief Destructor.
	 */
	virtual ~Drawable();

private:
	// the name of the file containing the texture
	std::string _texture;
};

inline Drawable::Drawable()
	: osg::Geometry()
{
}

inline Drawable::Drawable(const Drawable& src, const osg::CopyOp& copyOp)
	: osg::Geometry(src, copyOp), _texture(src._texture)
{
}

inline Drawable::~Drawable()
{
}

inline std::string Drawable::texture() const
{
	return _texture;
}

} } // namespace declarations

#endif  // __ORBIS_DRAWABLE_HPP__

