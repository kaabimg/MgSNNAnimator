var size= 15;
var radius = 10;

var d_angle = mgMath.pi()*2/size;
var id = 0;

for (var angle = 0; angle <  mgMath.pi()*2; angle+=d_angle)
{
    network.addNeuron(
        id,
        mgMath.createVector(
            radius * mgMath.cos(angle),
            radius * mgMath.sin(angle),
            1
        )
    );
    ++id;
}

var pointsSize= 20;
var tarjectoryRadius = 50 ;
var d_angle = mgMath.pi()*2/size;

for (var angle = 0; angle <  mgMath.pi()*2; angle+=d_angle)
{
    cameraAnimation.addCameraPositionPoint(	
        mgMath.createVector(
        	tarjectoryRadius * mgMath.cos(angle),
        	tarjectoryRadius * mgMath.sin(angle),
            1
        )
    );
}

cameraAnimation.addCameraCenterPoint(mgMath.createVector(5,5,5));
cameraAnimation.start();
