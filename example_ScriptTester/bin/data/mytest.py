#test
import random
from openframeworks import *

print "-"*30

class myApp(object):
	def __init__(self):
		self.x = 50.0
		self.y = 50.0
		self.d = 10
		self.fbo = ofFbo()
		self.fbo.allocate(100,100)
	
	def update(self):
		self.x += random.random()-.5
		self.y += random.random()-.5

	def draw(self):
		self.fbo.begin()
		ofClear(0)
		color = ofColor(255,0,0)
		ofSetColor(color)
		ofCircle(self.x,self.y,self.d)
		self.fbo.end()
		self.fbo.draw(200,200)

	def mouseMoved(self, x, y):
		pass

	def mousePressed(self, x, y, button):
		pass

	def mouseDragged(self, x, y, buton):
		pass

	def mouseReleased(self, x, y, button):
		pass

	def keyPressed(self, key):
		pass

	def keyReleased(self, key):
		pass
