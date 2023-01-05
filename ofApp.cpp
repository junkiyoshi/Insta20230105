#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(60);
	ofSetWindowTitle("openFrameworks");

	ofBackground(255);
	ofSetLineWidth(3);
	ofEnableDepthTest();

	this->frame.setMode(ofPrimitiveMode::OF_PRIMITIVE_LINES);
}
//--------------------------------------------------------------
void ofApp::update() {

	this->face.clear();
	this->frame.clear();

	for (int radius = 150; radius <= 270; radius += 5) {

		this->setRingToMesh(this->face, this->frame, glm::vec3(), radius, 60);
	}
}

//--------------------------------------------------------------
void ofApp::draw() {

	this->cam.begin();
	ofRotateX(90);
	ofRotateZ(ofGetFrameNum() * 0.666666666666666);

	ofSetColor(255);
	this->face.draw();

	ofSetColor(255, 0, 0);
	this->frame.drawWireframe();

	this->cam.end();
}

//--------------------------------------------------------------
void ofApp::setRingToMesh(ofMesh& face_target, ofMesh& frame_target, glm::vec3 location, float radius, float height) {

	int index = face_target.getNumVertices();

	for (int deg = 0; deg < 360; deg += 1) {

		vector<glm::vec3> vertices;
		vertices.push_back(glm::vec3(radius * cos(deg * DEG_TO_RAD), radius * sin(deg * DEG_TO_RAD), height * -0.5));
		vertices.push_back(glm::vec3(radius * cos((deg + 1) * DEG_TO_RAD), radius * sin((deg + 1) * DEG_TO_RAD), height * -0.5));
		vertices.push_back(glm::vec3(radius * cos((deg + 1) * DEG_TO_RAD), radius * sin((deg + 1) * DEG_TO_RAD), height * 0.5));
		vertices.push_back(glm::vec3(radius * cos(deg * DEG_TO_RAD), radius * sin(deg * DEG_TO_RAD), height * 0.5));

		for (auto& vertex : vertices) {

			auto noise_value = ofNoise(
				vertex.z * 0.001 + ofGetFrameNum() * 0.004,
				(abs)(vertex.x) * 0.001,
				(abs)(vertex.y) * 0.001);
			float param = 0.5;
			if (noise_value < 0.45) { param = ofMap(noise_value, 0, 0.45, 0, 0.5); }
			if (noise_value > 0.55) { param = ofMap(noise_value, 0.55, 1, 0.5, 1); }

			auto rotation = glm::rotate(glm::mat4(), ofMap(param, 0, 1, -PI * 2.5, PI * 2.5), glm::vec3(0, 1, 0));
			vertex = glm::vec4(vertex, 0) * rotation + glm::vec4(location, 0);
		}

		auto face_index = face_target.getNumVertices();
		face_target.addVertices(vertices);

		face_target.addIndex(face_index + 0); face_target.addIndex(face_index + 1); face_target.addIndex(face_index + 2);
		face_target.addIndex(face_index + 0); face_target.addIndex(face_index + 2); face_target.addIndex(face_index + 3);

		auto frame_index = frame_target.getNumVertices();
		frame_target.addVertices(vertices);

		frame_target.addIndex(frame_index + 0); frame_target.addIndex(frame_index + 1);
		frame_target.addIndex(frame_index + 2); frame_target.addIndex(frame_index + 3);
	}
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}