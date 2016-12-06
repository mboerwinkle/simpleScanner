echo "IMAGE CAPTURE (0) START"
0imgCap/out.out
echo "FINISH"
echo "LIGHT COORDINATE DETERMINATION (1) START"
1findCoords/out.out > shared/camView.csv
echo "FINISH"
echo "POINT CLOUD CREATION (2) START"
2createPointCloud/out.out < shared/camView.csv > shared/pointCloud.csv
echo "FINISH"
