/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
**
** Copyright (C), 2003,
**	Steve Quenette, 110 Victoria Street, Melbourne, Victoria, 3053, Australia.
**	Californian Institute of Technology, 1200 East California Boulevard, Pasadena, California, 91125, USA.
**	University of Texas, 1 University Station, Austin, Texas, 78712, USA.
**
** Authors:
**	Stevan M. Quenette, Senior Software Engineer, VPAC. (steve@vpac.org)
**	Stevan M. Quenette, Visitor in Geophysics, Caltech.
**	Luc Lavier, Research Scientist, The University of Texas. (luc@utig.ug.utexas.edu)
**	Luc Lavier, Research Scientist, Caltech.
**
** This program is free software; you can redistribute it and/or modify it
** under the terms of the GNU General Public License as published by the
** Free Software Foundation; either version 2, or (at your option) any
** later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program; if not, write to the Free Software
** Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
**
** $Id: InitialConditions.c 3261 2006-11-14 21:18:05Z EunseoChoi $
**
**~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#include <mpi.h>
#include <StGermain/StGermain.h>
#include <StGermain/FD/FD.h>
#include "Snac/Snac.h"
#include "units.h"
#include "types.h"
#include "Element.h"
#include "InitialConditions.h"
#include "Register.h"
#include <assert.h>
#include <string.h>
#include <math.h>
#define min(a,b) ((a)<(b) ? (a):(b))
#ifndef PATH_MAX
	#define PATH_MAX 1024
#endif

void SnacMaxwellDruckerPrager_InitialConditions( void* _context, void* data ) {
	Snac_Context*		context = (Snac_Context*)_context;
	Element_LocalIndex	element_lI;
	double                  mu,vis_min,dt_maxwellI;
	double                  dt_maxwell = 0;
	const double            mfrac      = 9.0e-01;

	int			restart = 0;
	Dictionary_Entry_Value* pluginsList;
	Dictionary_Entry_Value* plugin;
	if( context->rank == 0 ) Journal_Printf( context->snacInfo, "In: %s\n", __func__ );
// search for restart plugin
	pluginsList = PluginsManager_GetPluginsList( context->dictionary );
	if (pluginsList) {
		plugin = Dictionary_Entry_Value_GetFirstElement(pluginsList);
		while ( plugin ) {
			if ( 0 == strcmp( Dictionary_Entry_Value_AsString( plugin ),
					  "SnacRestart" ) ) {
				restart = 1;
				break;
			}
			plugin = plugin->next;
		}
	}

// loop over the phase using the dictionary
         
        Dictionary_Entry_Value* materialList = Dictionary_Get( context->dictionary, "materials" );
        int                           PhaseI = 0;
        if( materialList ) {
                Dictionary_Entry_Value* materialEntry = Dictionary_Entry_Value_GetFirstElement( materialList );
                /* loop around the  phases to initialize rheology */
                while( materialEntry ) {
                        context->materialProperty[PhaseI].rheology |= Snac_Material_MaxwellDruckerPrager;
                        mu                                          = context->materialProperty[PhaseI].mu;
                        vis_min                                     = context->materialProperty[PhaseI].vis_min;
                        dt_maxwellI                                 = mfrac*vis_min/mu;
			dt_maxwell                                  = (PhaseI==0)?dt_maxwellI:min(dt_maxwellI,dt_maxwell);
                        PhaseI++;
                        materialEntry                               = materialEntry->next;

                }
        }
        else {
                context->materialProperty[PhaseI].rheology |= Snac_Material_MaxwellDruckerPrager;
                mu                                          = context->materialProperty[PhaseI].mu;
               vis_min                                      = context->materialProperty[PhaseI].vis_min;
	       dt_maxwell                                   = mfrac*vis_min/mu;
        }
	if( context->dt > dt_maxwell) {
		if(context->dtType == Snac_DtType_Constant) 
			fprintf(stderr,"dt(%e) should be smaller than dt_maxwell(%e) (mu=%e vis_min=%e mfrac=%e)\n",context->dt,dt_maxwell,mu,vis_min,mfrac);
		else    context->dt = dt_maxwell;
        }
	if( restart ) {
		FILE*				fp1;
		FILE*				fp2;
		char				path[PATH_MAX];

		sprintf(path, "%s/snac.plStrainTensor.%d.%06d.restart",context->outputPath,context->rank,context->restartStep);
		Journal_Firewall( (fp1 = fopen(path,"r")) != NULL, "Can't find %s", path );

		sprintf(path, "%s/snac.plStrain.%d.%06d.restart",context->outputPath,context->rank,context->restartStep);
		Journal_Firewall( (fp2 = fopen(path,"r")) != NULL, "Can't find %s", path );

		/* read in restart file to construct the initial temperature */
		for( element_lI = 0; element_lI < context->mesh->elementLocalCount; element_lI++ ) {
			Snac_Element*		element = Snac_Element_At( context, element_lI );
			SnacMaxwellDruckerPrager_Element*	viscoplasticElement = ExtensionManager_Get( context->mesh->elementExtensionMgr, element,
														SnacMaxwellDruckerPrager_ElementHandle );
			const Snac_Material* material = &context->materialProperty[element->material_I];
			double		     plStrain;
			vis_min = context->materialProperty[element->material_I].vis_min;
			Tetrahedra_Index	tetra_I;
			double              depls = 0.0f;

			for( tetra_I = 0; tetra_I < Tetrahedra_Count; tetra_I++ ) {
				double			S[3][3];
				Index			i,j;
				double			depm;
				fscanf( fp1, "%le %le %le %le %le %le %le %le %le", &S[0][0],&S[0][1],&S[0][2],&S[1][0],&S[1][1],&S[1][2],&S[2][0],&S[2][1],&S[2][2]);
				for(i=0;i<3;i++)
					for(j=0;j<3;j++)
						viscoplasticElement->plasticstrainTensor[tetra_I][i][j] = S[i][j];
				/* not the actual viscosity at restartStep, but doesn't affect the calculation afterwards */
				viscoplasticElement->viscosity[tetra_I] = vis_min;
				
				depm = ( S[0][0]+S[1][1]+S[2][2] ) / 3.0f;
				viscoplasticElement->plasticStrain[tetra_I] = sqrt( 0.5f* ((S[0][0]-depm) * (S[0][0]-depm) + (S[1][1]-depm) * (S[1][1]-depm) + (S[2][2]-depm) * (S[2][2]-depm)) + S[0][1]*S[0][1] + S[0][2]*S[0][2] + S[1][2]*S[1][2]);
			} // for
			fscanf( fp2, "%le", &plStrain);
			viscoplasticElement->aps = plStrain;
		}// for
		if( fp1 )
			fclose( fp1 );
		if( fp2 )
			fclose( fp2 );
	}// if
	else {
		/* Set the plastic element initial conditions */
		for( element_lI = 0; element_lI < context->mesh->elementLocalCount; element_lI++ ) {
			Snac_Element*		element = Snac_Element_At( context, element_lI );
			SnacMaxwellDruckerPrager_Element*	viscoplasticElement = ExtensionManager_Get( context->mesh->elementExtensionMgr, element,
                                                                                                                SnacMaxwellDruckerPrager_ElementHandle );
                        double plStrain = viscoplasticElement->aps;
                        vis_min = context->materialProperty[element->material_I].vis_min;
			Tetrahedra_Index	tetra_I;
			for( tetra_I = 0; tetra_I < Tetrahedra_Count; tetra_I++ ) {
				viscoplasticElement->plasticStrain[tetra_I] = plStrain;
				viscoplasticElement->viscosity[tetra_I] = vis_min;
			}//for
		}//for 
}//else
}//function

