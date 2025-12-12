#include "PrimitiveAssembly.h"

//##From Powerpoint slides
// ========== Object Creation =============
//handles cube instantiation
void SpawnCube(int m, GLint texIDIX, Vector3 pos = Vector3(0, 0, 0), Vector3 scale = Vector3(1, 1, 1), Vector3 rot = Vector3(0, 0, 0), Vector3 color = Vector3(1, 1, 1)) {

	//Push New Matrix
	glPushMatrix();
	//Apply Color
	glColor3d(color.x, color.y, color.z);
	//Apply Translation
	glTranslated(pos.x, pos.y, pos.z);
	//Apply Rotation
	glRotated(rot.x, 1, 0, 0);
	glRotated(rot.y, 0, 1, 0);
	glRotated(rot.z, 0, 0, 1);

	//Apply Scale
	glScaled(scale.x, scale.y, scale.z);

	float half = 0.5f;

	switch (m) {
	case 0:
		//handles vertex cube
		glPointSize(8.0f); // make vertices visible
		glBegin(GL_POINTS);
		glVertex3f(-half, -half, -half);
		glVertex3f(half, -half, -half);
		glVertex3f(-half, half, -half);
		glVertex3f(half, half, -half);
		glVertex3f(-half, -half, half);
		glVertex3f(half, -half, half);
		glVertex3f(-half, half, half);
		glVertex3f(half, half, half);
		glEnd();

		break;
	case 1:
		//handles wire cube
		color = Vector3(1, 1, 1);
		glutWireCube(1);
		break;

	case 2:
		//handles solid cube
		glutSolidCube(1);
		break;
	case 3:
		// Enable texture mapping
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, texIDIX);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		// Draw a textured box
		glBegin(GL_QUADS);
		// Front face
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-half, -half, half);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(half, -half, half);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(half, half, half);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-half, half, half);
		// Back face
		glTexCoord2f(1.0f, 0.0f); glVertex3f(-half, -half, -half);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(-half, half, -half);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(half, half, -half);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(half, -half, -half);
		// Top face
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-half, half, -half);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-half, half, half);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(half, half, half);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(half, half, -half);
		// Bottom face
		glTexCoord2f(1.0f, 1.0f); glVertex3f(-half, -half, -half);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(half, -half, -half);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(half, -half, half);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(-half, -half, half);
		// Right face
		glTexCoord2f(1.0f, 0.0f); glVertex3f(half, -half, -half);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(half, half, -half);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(half, half, half);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(half, -half, half);
		// Left face
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-half, -half, -half);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(-half, -half, half);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(-half, half, half);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-half, half, -half);
		glEnd();
		glDisable(GL_TEXTURE_2D); // Disable texture mapping
		break;
	}

	glPopMatrix();
}

//handles sphere instantiation
void SpawnSphere(int m, GLint texIDIX, GLint slices = 10, GLint stacks = 10, Vector3 pos = Vector3(0, 0, 0), Vector3 scale = Vector3(1, 1, 1), Vector3 rot = Vector3(0, 0, 0), Vector3 color = Vector3(1, 1, 1)) {
	//Push New Matrix
	glPushMatrix();
	//Apply Color
	glColor3d(color.x, color.y, color.z);
	//Apply Translation
	glTranslated(pos.x, pos.y, pos.z);
	//Apply Rotation
	glRotated(rot.x, 1, 0, 0);
	glRotated(rot.y, 0, 1, 0);
	glRotated(rot.z, 0, 0, 1);

	//Apply Scale
	glScaled(scale.x, scale.y, scale.z);


	float radius = 1.0f;

	switch (m) {
	case 0:
		//handles vertex sphere
		glPointSize(4.0f); // make points visible

		glBegin(GL_POINTS);
		//interate through the stacks and get z and r locations
		for (int i = 0; i <= stacks; i++) {
			float lat = PI * (-0.5f + (float)i / stacks);
			float z = radius * sin(lat);
			float r = radius * cos(lat);

			//iterate through the slices and get the x and y locations
			for (int j = 0; j <= slices; j++) {
				float lon = 2 * PI * (float)j / slices;
				float x = r * cos(lon);
				float y = r * sin(lon);
				glVertex3f(x, y, z);
			}
		}
		glEnd();
		break;
	case 1:
		//handles wire sphere
		color = Vector3(1, 1, 1);
		glutWireSphere(1, slices, stacks);
		break;

	case 2:
		//handles solid sphere
		glutSolidSphere(1, slices, stacks);
		break;
	case 3:
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, texIDIX);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		for (int i = 0; i <= stacks; ++i) {
			float phi = PI * (float)i / (float)stacks; // Latitude angle
			float y = radius * cos(phi);
			float r_slice = radius * sin(phi); // Radius of this latitude slice

			glBegin(GL_TRIANGLE_STRIP);
			for (int j = 0; j <= slices; ++j) {
				float theta = 2.0f * PI * (float)j / (float)slices; //Longitude angle
				float x = r_slice * cos(theta);
				float z = r_slice * sin(theta);
				// Calculate UV coordinates
				// U: normalized longitude (0 to 1)
				// V: normalized latitude (0 to 1)
				float u = (float)j / (float)slices;
				float v = (float)i / (float)stacks;
				glTexCoord2f(u, v);
				glVertex3f(x, y, z);
				// For the next stack level
				if (i < stacks) {
					float next_phi = PI * (float)(i + 1) / (float)stacks;
					float next_y = radius * cos(next_phi);
					float next_r_slice = radius * sin(next_phi);
					float next_x = next_r_slice * cos(theta);
					float next_z = next_r_slice * sin(theta);
					glTexCoord2f(u, (float)(i + 1) / (float)stacks);
					glVertex3f(next_x, next_y, next_z);
				}
			}
			glEnd();
		}
		glDisable(GL_TEXTURE_2D);

		break;
	}

	//Reset State
	glPopMatrix();
}