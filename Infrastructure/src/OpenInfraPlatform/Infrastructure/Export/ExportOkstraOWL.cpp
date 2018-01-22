/*
    Copyright (c) 2018 Technical University of Munich
    Chair of Computational Modeling and Simulation.

    TUM Open Infra Platform is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License Version 3
    as published by the Free Software Foundation.

    TUM Open Infra Platform is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#include "ExportOkstraOWL.h"
#include <fstream>

#include "raptor2/raptor2.h"

static raptor_serializer* rdf_serializer;

// static void serialize_triple(void* user_data, raptor_statement* triple) {
// 	raptor_serializer_serialize_statement(rdf_serializer, triple);
// }
//
// static void declare_namespace(void* user_data, raptor_namespace* nspace) {
// 	raptor_serializer_set_namespace_from_namespace(rdf_serializer, nspace);
// }

OpenInfraPlatform::Infrastructure::ExportOkstraOWL::ExportOkstraOWL(buw::ReferenceCounted<buw::AlignmentModel> am,
                                                                    buw::ReferenceCounted<buw::DigitalElevationModel> dem,
                                                                    const std::string& filename)
    : Export(am, dem, filename) {
	// foaf test
	{
		FILE* outfile = fopen(filename.c_str(), "w");
		raptor_world* world = raptor_new_world();
		rdf_serializer = raptor_new_serializer(world, "rdfxml" /* "turtle" */);
		raptor_serializer_start_to_file_handle(rdf_serializer, nullptr, outfile);

		const unsigned char* prefix = (const unsigned char*)"foaf";
		raptor_uri* uri = raptor_new_uri(world, (const unsigned char*)"http://xmlns.com/foaf/0.1/");
		raptor_serializer_set_namespace(rdf_serializer, uri, prefix);

		raptor_statement* triple = nullptr;
		triple = raptor_new_statement(world);
		triple->subject = raptor_new_term_from_uri_string(world, (const unsigned char*)"http://xmlns.com/foaf/0.1/Person");
		triple->predicate = raptor_new_term_from_uri_string(world, (const unsigned char*)"http://xmlns.com/foaf/0.1/Person/name");
		std::string name = "Jimmy Wales";
		triple->object = raptor_new_term_from_literal(world, (unsigned char*)name.c_str(), nullptr, nullptr);
		raptor_serializer_serialize_statement(rdf_serializer, triple);
		raptor_free_statement(triple);

		raptor_serializer_serialize_end(rdf_serializer);
		raptor_free_serializer(rdf_serializer);
		raptor_free_world(world);

		fclose(outfile);

		return;
	}

	FILE* outfile = fopen(filename.c_str(), "w");

	raptor_world* world = raptor_new_world();

	rdf_serializer = raptor_new_serializer(world, "turtle");
	raptor_serializer_start_to_file_handle(rdf_serializer, nullptr, outfile);

	const unsigned char* prefix = (const unsigned char*)"okstra";
	raptor_uri* uri = raptor_new_uri(world, (const unsigned char*)"Data/okstra#");
	raptor_serializer_set_namespace(rdf_serializer, uri, prefix);

	for (int i = 0; i < am->getAlignmentCount(); i++) {
		buw::String id = am->getAlignments()[i]->getName();

		raptor_statement* triple = nullptr;

		triple = raptor_new_statement(world);
		triple->subject = raptor_new_term_from_uri_string(world, (const unsigned char*)"Data/okstra#Hauptachse");
		triple->predicate = raptor_new_term_from_uri_string(world, (const unsigned char*)"Data/okstra#hat_Achselement_Achse");
		triple->object = raptor_new_term_from_uri_string(world, (const unsigned char*)"Data/okstra#Achselement_List");
		raptor_serializer_serialize_statement(rdf_serializer, triple);
		raptor_free_statement(triple);

		triple = raptor_new_statement(world);
		triple->subject = raptor_new_term_from_uri_string(world, (const unsigned char*)"Data/okstra#Hauptachse");
		triple->predicate = raptor_new_term_from_uri_string(world, (const unsigned char*)"Data/okstra#Kennung");
		triple->object = raptor_new_term_from_literal(world, (unsigned char*)id.toCString(), nullptr, nullptr);
		raptor_serializer_serialize_statement(rdf_serializer, triple);
		raptor_free_statement(triple);

		triple = raptor_new_statement(world);
		triple->subject = raptor_new_term_from_uri_string(world, (const unsigned char*)"Data/okstra#Hauptachse");
		triple->predicate = raptor_new_term_from_uri_string(world, (const unsigned char*)"Data/okstra#Langtext");
		triple->object = raptor_new_term_from_literal(world, (const unsigned char*)"Hauptachse 1", nullptr, nullptr);
		raptor_serializer_serialize_statement(rdf_serializer, triple);
		raptor_free_statement(triple);
	}

	if (dem->hasSurfaces()) {
		raptor_statement* triple = nullptr;

		triple = raptor_new_statement(world);
		triple->subject = raptor_new_term_from_uri_string(world, (const unsigned char*)"Data/okstra#DGM");
		triple->predicate = raptor_new_term_from_uri_string(world, (const unsigned char*)"Data/okstra#Bezeichnung");
		triple->object = raptor_new_term_from_uri_string(world, (const unsigned char*)"Data/okstra#Gel�ndemodell");
		raptor_serializer_serialize_statement(rdf_serializer, triple);
		raptor_free_statement(triple);

		for (int i = 0; i < dem->getSurfaceCount(); i++) {
		}
	}

	raptor_serializer_serialize_end(rdf_serializer);

	raptor_free_serializer(rdf_serializer);
	raptor_free_world(world);

	fclose(outfile);
}

OpenInfraPlatform::Infrastructure::ExportOkstraOWL::~ExportOkstraOWL() {
}