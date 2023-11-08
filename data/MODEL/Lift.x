xof 0302txt 0064
template Header {
 <3D82AB43-62DA-11cf-AB39-0020AF71E433>
 WORD major;
 WORD minor;
 DWORD flags;
}

template Vector {
 <3D82AB5E-62DA-11cf-AB39-0020AF71E433>
 FLOAT x;
 FLOAT y;
 FLOAT z;
}

template Coords2d {
 <F6F23F44-7686-11cf-8F52-0040333594A3>
 FLOAT u;
 FLOAT v;
}

template Matrix4x4 {
 <F6F23F45-7686-11cf-8F52-0040333594A3>
 array FLOAT matrix[16];
}

template ColorRGBA {
 <35FF44E0-6C7C-11cf-8F52-0040333594A3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
 FLOAT alpha;
}

template ColorRGB {
 <D3E16E81-7835-11cf-8F52-0040333594A3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
}

template IndexedColor {
 <1630B820-7842-11cf-8F52-0040333594A3>
 DWORD index;
 ColorRGBA indexColor;
}

template Boolean {
 <4885AE61-78E8-11cf-8F52-0040333594A3>
 WORD truefalse;
}

template Boolean2d {
 <4885AE63-78E8-11cf-8F52-0040333594A3>
 Boolean u;
 Boolean v;
}

template MaterialWrap {
 <4885AE60-78E8-11cf-8F52-0040333594A3>
 Boolean u;
 Boolean v;
}

template TextureFilename {
 <A42790E1-7810-11cf-8F52-0040333594A3>
 STRING filename;
}

template Material {
 <3D82AB4D-62DA-11cf-AB39-0020AF71E433>
 ColorRGBA faceColor;
 FLOAT power;
 ColorRGB specularColor;
 ColorRGB emissiveColor;
 [...]
}

template MeshFace {
 <3D82AB5F-62DA-11cf-AB39-0020AF71E433>
 DWORD nFaceVertexIndices;
 array DWORD faceVertexIndices[nFaceVertexIndices];
}

template MeshFaceWraps {
 <4885AE62-78E8-11cf-8F52-0040333594A3>
 DWORD nFaceWrapValues;
 Boolean2d faceWrapValues;
}

template MeshTextureCoords {
 <F6F23F40-7686-11cf-8F52-0040333594A3>
 DWORD nTextureCoords;
 array Coords2d textureCoords[nTextureCoords];
}

template MeshMaterialList {
 <F6F23F42-7686-11cf-8F52-0040333594A3>
 DWORD nMaterials;
 DWORD nFaceIndexes;
 array DWORD faceIndexes[nFaceIndexes];
 [Material]
}

template MeshNormals {
 <F6F23F43-7686-11cf-8F52-0040333594A3>
 DWORD nNormals;
 array Vector normals[nNormals];
 DWORD nFaceNormals;
 array MeshFace faceNormals[nFaceNormals];
}

template MeshVertexColors {
 <1630B821-7842-11cf-8F52-0040333594A3>
 DWORD nVertexColors;
 array IndexedColor vertexColors[nVertexColors];
}

template Mesh {
 <3D82AB44-62DA-11cf-AB39-0020AF71E433>
 DWORD nVertices;
 array Vector vertices[nVertices];
 DWORD nFaces;
 array MeshFace faces[nFaces];
 [...]
}

Header{
1;
0;
1;
}

Mesh {
 118;
 10.45212;6.39992;-6.00000;,
 10.45212;6.39992;6.00000;,
 12.74823;5.91275;6.00000;,
 12.74823;5.91275;-6.00000;,
 14.69475;4.52539;6.00000;,
 14.69475;4.52539;-6.00000;,
 15.99540;2.44910;6.00000;,
 15.99540;2.44910;-6.00000;,
 10.45212;-6.40008;6.00000;,
 10.45212;-6.40008;-6.00000;,
 12.74823;-5.91292;-6.00000;,
 12.74823;-5.91292;6.00000;,
 14.69475;-4.52556;-6.00000;,
 14.69475;-4.52556;6.00000;,
 15.99540;-2.44927;-6.00000;,
 15.99540;-2.44927;6.00000;,
 -10.45212;6.39992;6.00000;,
 -10.45212;6.39992;-6.00000;,
 -12.74823;5.91275;-6.00000;,
 -12.74823;5.91275;6.00000;,
 -14.69475;4.52539;-6.00000;,
 -14.69475;4.52539;6.00000;,
 -15.99540;2.44910;-6.00000;,
 -15.99540;2.44910;6.00000;,
 -10.45212;-6.40008;-6.00000;,
 -10.45212;-6.40008;6.00000;,
 -12.74823;-5.91292;6.00000;,
 -12.74823;-5.91292;-6.00000;,
 -14.69475;-4.52556;6.00000;,
 -14.69475;-4.52556;-6.00000;,
 -15.99540;-2.44927;6.00000;,
 -15.99540;-2.44927;-6.00000;,
 10.45212;5.91275;-6.76537;,
 15.25275;2.44910;-6.92388;,
 10.45212;4.52539;-7.41421;,
 13.22376;2.44910;-7.60021;,
 10.45212;2.44910;-7.84776;,
 -10.45212;5.91275;-6.76537;,
 -15.25275;2.44910;-6.92388;,
 -10.45212;2.44910;-7.84776;,
 -13.22376;2.44910;-7.60021;,
 -10.45212;4.52539;-7.41421;,
 10.45212;-5.91292;-6.76537;,
 15.25275;-2.44927;-6.92388;,
 10.45212;-2.44927;-7.84776;,
 13.22376;-2.44927;-7.60021;,
 10.45212;-4.52556;-7.41421;,
 10.45212;5.91275;6.76537;,
 15.25275;2.44910;6.92388;,
 10.45212;2.44910;7.84776;,
 13.22376;2.44910;7.60021;,
 10.45212;4.52539;7.41421;,
 -10.45212;-5.91292;-6.76537;,
 -15.25275;-2.44927;-6.92388;,
 -10.45212;-4.52556;-7.41421;,
 -13.22376;-2.44927;-7.60021;,
 -10.45212;-2.44927;-7.84776;,
 10.45212;-5.91292;6.76537;,
 15.25275;-2.44927;6.92388;,
 10.45212;-4.52556;7.41421;,
 13.22376;-2.44927;7.60021;,
 10.45212;-2.44927;7.84776;,
 -10.45212;5.91275;6.76537;,
 -15.25275;2.44910;6.92388;,
 -10.45212;4.52539;7.41421;,
 -13.22376;2.44910;7.60021;,
 -10.45212;2.44910;7.84776;,
 -10.45212;-5.91292;6.76537;,
 -15.25275;-2.44927;6.92388;,
 -10.45212;-2.44927;7.84776;,
 -13.22376;-2.44927;7.60021;,
 -10.45212;-4.52556;7.41421;,
 -6.95298;4.52472;-7.41400;,
 -6.95298;2.44840;-7.84755;,
 -6.95298;5.91208;-6.76515;,
 -6.95298;6.39924;-5.99979;,
 -6.95298;6.39924;-5.99979;,
 -10.45212;6.39992;-6.00000;,
 -6.95298;6.39924;6.00021;,
 -6.95298;-2.44994;-7.84755;,
 -6.95298;-4.52626;-7.41400;,
 -6.95298;6.39924;6.00021;,
 -6.95298;5.91208;6.76558;,
 -6.95298;-5.91359;-6.76515;,
 -6.95298;4.52472;7.41443;,
 -6.95298;-6.40076;-5.99979;,
 -6.95298;2.44840;7.84797;,
 -6.95298;-6.40076;-5.99979;,
 -6.95298;-6.40076;6.00021;,
 -10.45212;-6.40008;6.00000;,
 -6.95298;-6.40076;6.00021;,
 -6.95298;-5.91359;6.76558;,
 -6.95298;-4.52626;7.41443;,
 -6.95298;-2.44994;7.84797;,
 6.95298;4.52472;-7.41400;,
 6.95298;2.44840;-7.84755;,
 6.95298;5.91208;-6.76515;,
 6.95298;6.39924;-5.99979;,
 6.95298;6.39924;-5.99979;,
 6.95298;6.39924;6.00021;,
 10.45212;6.39992;6.00000;,
 10.45212;6.39992;-6.00000;,
 6.95298;-2.44994;-7.84755;,
 6.95298;-4.52626;-7.41400;,
 6.95298;6.39924;6.00021;,
 6.95298;5.91208;6.76558;,
 6.95298;-5.91359;-6.76515;,
 6.95298;4.52472;7.41443;,
 6.95298;-6.40076;-5.99979;,
 6.95298;2.44840;7.84797;,
 6.95298;-6.40076;-5.99979;,
 10.45212;-6.40008;-6.00000;,
 10.45212;-6.40008;6.00000;,
 6.95298;-6.40076;6.00021;,
 6.95298;-6.40076;6.00021;,
 6.95298;-5.91359;6.76558;,
 6.95298;-4.52626;7.41443;,
 6.95298;-2.44994;7.84797;;
 
 114;
 4;0,1,2,3;,
 4;3,2,4,5;,
 4;5,4,6,7;,
 4;8,9,10,11;,
 4;11,10,12,13;,
 4;13,12,14,15;,
 4;16,17,18,19;,
 4;19,18,20,21;,
 4;21,20,22,23;,
 4;24,25,26,27;,
 4;27,26,28,29;,
 4;29,28,30,31;,
 3;0,3,32;,
 3;5,7,33;,
 3;34,35,36;,
 3;37,18,17;,
 3;38,22,20;,
 3;39,40,41;,
 3;42,10,9;,
 3;43,14,12;,
 3;44,45,46;,
 3;47,2,1;,
 3;48,6,4;,
 3;49,50,51;,
 3;24,27,52;,
 3;29,31,53;,
 3;54,55,56;,
 3;8,11,57;,
 3;13,15,58;,
 3;59,60,61;,
 3;16,19,62;,
 3;21,23,63;,
 3;64,65,66;,
 3;67,26,25;,
 3;68,30,28;,
 3;69,70,71;,
 4;72,73,39,41;,
 4;72,41,37,74;,
 4;74,37,17,75;,
 4;76,77,16,78;,
 4;79,80,54,56;,
 4;81,16,62,82;,
 4;80,83,52,54;,
 4;82,62,64,84;,
 4;83,85,24,52;,
 4;84,64,66,86;,
 4;87,88,89,24;,
 4;90,91,67,25;,
 4;91,92,71,67;,
 4;93,69,71,92;,
 4;94,34,36,95;,
 4;94,95,73,72;,
 4;94,72,74,96;,
 4;94,96,32,34;,
 4;96,74,75,97;,
 4;96,97,0,32;,
 4;98,76,78,99;,
 4;98,99,100,101;,
 4;102,44,46,103;,
 4;102,103,80,79;,
 4;104,81,82,105;,
 4;104,105,47,1;,
 4;103,46,42,106;,
 4;103,106,83,80;,
 4;105,82,84,107;,
 4;105,107,51,47;,
 4;106,42,9,108;,
 4;106,108,85,83;,
 4;107,84,86,109;,
 4;107,109,49,51;,
 4;110,111,112,113;,
 4;110,113,88,87;,
 4;114,8,57,115;,
 4;114,115,91,90;,
 4;115,57,59,116;,
 4;115,116,92,91;,
 4;117,93,92,116;,
 4;117,116,59,61;,
 4;49,61,60,50;,
 4;109,117,61,49;,
 4;86,93,117,109;,
 4;66,69,93,86;,
 4;65,70,69,66;,
 4;63,68,70,65;,
 4;30,68,63,23;,
 4;22,31,30,23;,
 4;22,38,53,31;,
 4;40,55,53,38;,
 4;56,55,40,39;,
 4;73,79,56,39;,
 4;95,102,79,73;,
 4;36,44,102,95;,
 4;35,45,44,36;,
 4;33,43,45,35;,
 4;7,14,43,33;,
 4;6,15,14,7;,
 4;48,58,15,6;,
 4;50,60,58,48;,
 4;64,21,63,65;,
 4;19,21,64,62;,
 4;68,28,71,70;,
 4;28,26,67,71;,
 4;54,52,27,29;,
 4;53,55,54,29;,
 4;20,41,40,38;,
 4;37,41,20,18;,
 4;34,5,33,35;,
 4;32,3,5,34;,
 4;45,43,12,46;,
 4;46,12,10,42;,
 4;58,60,59,13;,
 4;13,59,57,11;,
 4;4,51,50,48;,
 4;2,47,51,4;;
 
 MeshMaterialList {
  4;
  114;
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  0,
  0,
  1,
  0,
  1,
  0,
  1,
  1,
  0,
  0,
  1,
  1,
  0,
  0,
  1,
  1,
  0,
  0,
  1,
  1,
  0,
  1,
  0,
  1,
  0,
  0,
  1,
  0,
  1,
  3,
  1,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  1,
  3,
  1,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0;;
  Material {
   0.480000;0.800000;0.000000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.473600;0.800000;0.696800;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.000000;0.646400;0.429600;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
 }
 MeshNormals {
  104;
  -0.100272;0.955141;0.278660;,
  0.100272;0.955141;0.278660;,
  -0.100255;0.955142;-0.278661;,
  0.100255;0.955142;-0.278661;,
  -0.100438;-0.955125;-0.278656;,
  0.100438;-0.955125;-0.278656;,
  -0.100455;-0.955123;0.278655;,
  0.100455;-0.955123;0.278655;,
  -0.121926;0.656483;-0.744422;,
  0.121926;0.656483;-0.744422;,
  -0.133745;0.343434;-0.929605;,
  0.133745;0.343434;-0.929605;,
  -0.044252;0.102648;-0.993733;,
  0.044252;0.102648;-0.993733;,
  -0.122052;-0.656473;-0.744410;,
  0.122052;-0.656473;-0.744410;,
  -0.133793;-0.343432;-0.929599;,
  0.133793;-0.343432;-0.929599;,
  -0.044272;-0.102648;-0.993732;,
  0.044272;-0.102648;-0.993732;,
  -0.121972;0.656480;0.744418;,
  0.121972;0.656480;0.744418;,
  -0.133791;0.343431;0.929599;,
  0.133791;0.343431;0.929599;,
  -0.044311;0.102647;0.993730;,
  0.044311;0.102647;0.993730;,
  -0.122098;-0.656470;0.744405;,
  0.122098;-0.656470;0.744405;,
  -0.133839;-0.343429;0.929593;,
  0.133839;-0.343429;0.929593;,
  -0.044332;-0.102647;0.993730;,
  0.044332;-0.102647;0.993730;,
  0.351787;0.850141;0.391798;,
  0.351787;0.850141;-0.391798;,
  0.612114;0.599064;0.516177;,
  0.612114;0.599064;-0.516177;,
  0.908066;0.264625;0.324639;,
  0.908066;0.264625;-0.324639;,
  0.351788;-0.850141;0.391798;,
  0.351788;-0.850141;-0.391798;,
  0.612114;-0.599064;0.516177;,
  0.612114;-0.599064;-0.516177;,
  0.908066;-0.264625;0.324639;,
  0.908066;-0.264625;-0.324639;,
  -0.351787;0.850141;0.391798;,
  -0.351787;0.850141;-0.391798;,
  -0.612114;0.599064;0.516177;,
  -0.612114;0.599064;-0.516177;,
  -0.908066;0.264625;0.324639;,
  -0.908066;0.264625;-0.324639;,
  -0.351788;-0.850141;0.391798;,
  -0.351788;-0.850141;-0.391798;,
  -0.612114;-0.599064;0.516177;,
  -0.612114;-0.599064;-0.516177;,
  -0.908066;-0.264625;0.324639;,
  -0.908066;-0.264625;-0.324639;,
  0.555021;0.237982;-0.797067;,
  0.198528;0.168169;-0.965560;,
  -0.555021;0.237982;-0.797067;,
  -0.198528;0.168169;-0.965560;,
  0.555021;-0.237982;-0.797067;,
  0.198528;-0.168169;-0.965560;,
  0.555021;0.237982;0.797067;,
  0.198528;0.168168;0.965560;,
  -0.555021;-0.237982;-0.797067;,
  -0.198528;-0.168169;-0.965560;,
  0.555021;-0.237982;0.797067;,
  0.198528;-0.168168;0.965560;,
  -0.555021;0.237982;0.797067;,
  -0.198528;0.168168;0.965560;,
  -0.555021;-0.237982;0.797067;,
  -0.198528;-0.168168;0.965560;,
  0.000059;0.316139;-0.948713;,
  0.000040;0.102743;-0.994708;,
  0.000086;0.659918;-0.751338;,
  0.000101;0.960105;-0.279641;,
  0.000020;-0.102743;-0.994708;,
  0.000084;0.960105;0.279638;,
  -0.000002;-0.316142;-0.948712;,
  0.000040;0.659920;0.751336;,
  -0.000041;-0.659921;-0.751335;,
  0.000001;0.316137;0.948714;,
  -0.000083;-0.960105;-0.279641;,
  -0.000020;0.102742;0.994708;,
  -0.000101;-0.960105;0.279638;,
  -0.000087;-0.659922;0.751334;,
  -0.000040;-0.102742;0.994708;,
  -0.000060;-0.316140;0.948713;,
  -0.000059;0.316139;-0.948713;,
  -0.000040;0.102743;-0.994708;,
  -0.000086;0.659918;-0.751337;,
  -0.000101;0.960105;-0.279641;,
  -0.000020;-0.102743;-0.994708;,
  -0.000084;0.960105;0.279638;,
  0.000002;-0.316142;-0.948712;,
  -0.000040;0.659920;0.751336;,
  0.000041;-0.659921;-0.751335;,
  -0.000001;0.316137;0.948714;,
  0.000083;-0.960105;-0.279641;,
  0.000020;0.102742;0.994708;,
  0.000101;-0.960105;0.279638;,
  0.000087;-0.659922;0.751334;,
  0.000040;-0.102742;0.994708;,
  0.000060;-0.316140;0.948712;;
  114;
  4;3,1,32,33;,
  4;33,32,34,35;,
  4;35,34,36,37;,
  4;7,5,39,38;,
  4;38,39,41,40;,
  4;40,41,43,42;,
  4;0,2,45,44;,
  4;44,45,47,46;,
  4;46,47,49,48;,
  4;4,6,50,51;,
  4;51,50,52,53;,
  4;53,52,54,55;,
  3;3,33,9;,
  3;35,37,56;,
  3;11,57,13;,
  3;8,45,2;,
  3;58,49,47;,
  3;12,59,10;,
  3;15,39,5;,
  3;60,43,41;,
  3;19,61,17;,
  3;21,32,1;,
  3;62,36,34;,
  3;25,63,23;,
  3;4,51,14;,
  3;53,55,64;,
  3;16,65,18;,
  3;7,38,27;,
  3;40,42,66;,
  3;29,67,31;,
  3;0,44,20;,
  3;46,48,68;,
  3;22,69,24;,
  3;26,50,6;,
  3;70,54,52;,
  3;30,71,28;,
  4;72,73,12,10;,
  4;72,10,8,74;,
  4;74,8,2,75;,
  4;75,2,0,77;,
  4;76,78,16,18;,
  4;77,0,20,79;,
  4;78,80,14,16;,
  4;79,20,22,81;,
  4;80,82,4,14;,
  4;81,22,24,83;,
  4;82,84,6,4;,
  4;84,85,26,6;,
  4;85,87,28,26;,
  4;86,30,28,87;,
  4;88,11,13,89;,
  4;88,89,73,72;,
  4;88,72,74,90;,
  4;88,90,9,11;,
  4;90,74,75,91;,
  4;90,91,3,9;,
  4;91,75,77,93;,
  4;91,93,1,3;,
  4;92,19,17,94;,
  4;92,94,78,76;,
  4;93,77,79,95;,
  4;93,95,21,1;,
  4;94,17,15,96;,
  4;94,96,80,78;,
  4;95,79,81,97;,
  4;95,97,23,21;,
  4;96,15,5,98;,
  4;96,98,82,80;,
  4;97,81,83,99;,
  4;97,99,25,23;,
  4;98,5,7,100;,
  4;98,100,84,82;,
  4;100,7,27,101;,
  4;100,101,85,84;,
  4;101,27,29,103;,
  4;101,103,87,85;,
  4;102,86,87,103;,
  4;102,103,29,31;,
  4;25,31,67,63;,
  4;99,102,31,25;,
  4;83,86,102,99;,
  4;24,30,86,83;,
  4;69,71,30,24;,
  4;68,70,71,69;,
  4;54,70,68,48;,
  4;49,55,54,48;,
  4;49,58,64,55;,
  4;59,65,64,58;,
  4;18,65,59,12;,
  4;73,76,18,12;,
  4;89,92,76,73;,
  4;13,19,92,89;,
  4;57,61,19,13;,
  4;56,60,61,57;,
  4;37,43,60,56;,
  4;36,42,43,37;,
  4;62,66,42,36;,
  4;63,67,66,62;,
  4;22,46,68,69;,
  4;44,46,22,20;,
  4;70,52,28,71;,
  4;52,50,26,28;,
  4;16,14,51,53;,
  4;64,65,16,53;,
  4;47,10,59,58;,
  4;8,10,47,45;,
  4;11,35,56,57;,
  4;9,33,35,11;,
  4;61,60,41,17;,
  4;17,41,39,15;,
  4;66,67,29,40;,
  4;40,29,27,38;,
  4;34,23,63,62;,
  4;32,21,23,34;;
 }
 MeshTextureCoords {
  118;
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.351190;1.000000;,
  0.000000;1.000000;,
  0.351190;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.351190;0.000000;,
  0.351190;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.573410;1.000000;,
  0.573410;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.573410;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.573410;1.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;;
 }
}
