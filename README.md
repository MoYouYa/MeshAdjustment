一个简单的曲面细分和合并的库

v1.01
	新实现了Catmull-Clark Subdivision方法，可以在MeshSubdivision(mesh, "filePath", SubFunction::Catmull_Clark_Subdivision);使用时选择此细分方式
 
v1.00
	定义了部分需要用到的类和枚举类型。
	简单实现了加载和生成 .obj 文件的功能，目前仅实现了将加载的.obj文件的面转换成三角形的功能。
	目前仅实现了将由三角形组成的mesh进行LoopSubdivision的方法，且新生成的mesh仅保留顶点位置和三角形索引。
