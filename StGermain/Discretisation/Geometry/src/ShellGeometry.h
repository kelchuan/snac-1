/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
**
** Copyright (C), 2003, Victorian Partnership for Advanced Computing (VPAC) Ltd, 110 Victoria Street, Melbourne, 3053, Australia.
**
** Authors:
**	Stevan M. Quenette, Senior Software Engineer, VPAC. (steve@vpac.org)
**	Patrick D. Sunter, Software Engineer, VPAC. (pds@vpac.org)
**	Luke J. Hodkinson, Computational Engineer, VPAC. (lhodkins@vpac.org)
**	Siew-Ching Tan, Software Engineer, VPAC. (siew@vpac.org)
**	Alan H. Lo, Computational Engineer, VPAC. (alan@vpac.org)
**	Raquibul Hassan, Computational Engineer, VPAC. (raq@vpac.org)
**
**  This library is free software; you can redistribute it and/or
**  modify it under the terms of the GNU Lesser General Public
**  License as published by the Free Software Foundation; either
**  version 2.1 of the License, or (at your option) any later version.
**
**  This library is distributed in the hope that it will be useful,
**  but WITHOUT ANY WARRANTY; without even the implied warranty of
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
**  Lesser General Public License for more details.
**
**  You should have received a copy of the GNU Lesser General Public
**  License along with this library; if not, write to the Free Software
**  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
**
*/
/** \file
**  Role:
**
** Assumptions:
**
** Comments: ) {
	IrregTopology* self = (IrregTopology*)ir
**
** $Id: ShellGeometry.h 3851 2006-10-12 08:57:22Z SteveQuenette $
**
**~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#ifndef __Discretisation_Geometry_ShellGeometry_h__
#define __Discretisation_Geometry_ShellGeometry_h__

	/* Virtual function types */
	
	/* Textual name of this class */
	extern const Type ShellGeometry_Type;

	/* ShellGeometry information */
	#define __ShellGeometry \
		/* General info */ \
		__Geometry \
		\
		/* Virtual info */ \
		\
		/* ShellGeometry info ... */ \
		IJK				size; \
		XYZ				min; \
		XYZ				max;
	struct ShellGeometry { __ShellGeometry };
	
	
	/*--------------------------------------------------------------------------------------------------------------------------
	** Constructors
	*/
	
	/* Create a ShellGeometry */
	ShellGeometry* ShellGeometry_DefaultNew( Name name );
	
	ShellGeometry* ShellGeometry_New(
		Name name,
		Dictionary*					dictionary );
	
	/* Initialise a ShellGeometry */
	void ShellGeometry_Init(
		ShellGeometry*					self,
		Name						name,
		Dictionary*					dictionary );
	
	/* Creation implementation */
	ShellGeometry* _ShellGeometry_New(
		SizeT						_sizeOfSelf, 
		Type						type,
		Stg_Class_DeleteFunction*				_delete,
		Stg_Class_PrintFunction*				_print,
		Stg_Class_CopyFunction*				_copy, 
		Stg_Component_DefaultConstructorFunction*	_defaultConstructor,
		Stg_Component_ConstructFunction*			_construct,
		Stg_Component_BuildFunction*		_build,
		Stg_Component_InitialiseFunction*		_initialise,
		Stg_Component_ExecuteFunction*		_execute,
		Stg_Component_DestroyFunction*		_destroy,
		Name							name,
		Bool							initFlag,
		Geometry_BuildPointsFunction*			buildPoints,
		Geometry_PointAtFunction*			pointAt,
		Dictionary*					dictionary );
	
	/* Initialisation implementation functions */
	void _ShellGeometry_Init(
		ShellGeometry*					self );
	
	
	/*--------------------------------------------------------------------------------------------------------------------------
	** Virtual functions
	*/
	
	/* Stg_Class_Delete ShellGeometry implementation */
	void _ShellGeometry_Delete( void* shellGeometry );
	
	/* Print ShellGeometry implementation */
	void _ShellGeometry_Print( void* shellGeometry, Stream* stream );
	
	void _ShellGeometry_Construct( void* shellGeometry, Stg_ComponentFactory *cf, void* data );
	
	void _ShellGeometry_Build( void* shellGeometry, void *data );
	
	void _ShellGeometry_Initialise( void* shellGeometry, void *data );
	
	void _ShellGeometry_Execute( void* shellGeometry, void *data );

	void _ShellGeometry_Destroy( void* shellGeometry, void *data );
	
	void _ShellGeometry_BuildPoints( void* shellGeometry, Coord_List points );
	
	void _ShellGeometry_PointAt( void* shellGeometry, Index index, Coord point );
	
	
	/*--------------------------------------------------------------------------------------------------------------------------
	** Public member functions
	*/
	
	
	/*--------------------------------------------------------------------------------------------------------------------------
	** Private Member functions
	*/
	
	
#endif /* __Discretisation_Geometry_ShellGeometry_h__ */
