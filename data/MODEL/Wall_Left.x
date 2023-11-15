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
 66;
 -118.34748;284.16626;-2.58887;,
 -116.75936;266.18542;14.68198;,
 -65.91311;253.28607;3.14890;,
 -76.98798;227.49390;47.42171;,
 -126.76701;264.43661;50.60903;,
 -90.64365;193.78850;91.33421;,
 -70.00365;179.06200;78.70470;,
 -128.08682;237.39972;85.48878;,
 -124.34454;192.19769;106.99515;,
 -53.65701;200.52228;39.25853;,
 -56.71184;118.70618;83.51847;,
 -35.38662;147.43103;43.68948;,
 -83.66436;139.21478;105.38073;,
 -140.96594;27.03076;141.35907;,
 -82.01145;86.27969;117.46004;,
 -66.23006;43.53564;102.50104;,
 -127.56540;-28.62634;139.35146;,
 -83.27020;8.38612;125.04472;,
 -32.85995;-140.75752;46.37744;,
 -54.75773;-143.49075;101.67464;,
 -68.26086;-172.40912;68.79298;,
 -24.57355;-140.75752;-3.47468;,
 -103.80323;-224.46017;-54.79928;,
 -81.71136;-226.45796;-20.71682;,
 -129.23264;-269.13593;7.58654;,
 -78.98472;-197.64597;50.33451;,
 -110.06393;-197.64597;66.12896;,
 1.35286;33.72385;-2.15590;,
 -9.15331;-31.17612;-42.02654;,
 -26.29058;109.35755;-39.11230;,
 -13.09066;72.16835;46.54142;,
 -2.37002;21.74694;70.00339;,
 -49.53403;218.28799;-48.15021;,
 -33.01874;150.63963;3.26777;,
 -68.11953;198.23268;-79.75165;,
 -76.56573;250.01653;-44.02418;,
 -56.81886;-192.97415;19.30505;,
 -65.08375;-203.30667;-26.18968;,
 -56.76666;-116.75615;-80.60550;,
 -32.85995;-140.75752;-53.32682;,
 -42.22549;-55.17056;-74.04517;,
 -39.39723;26.03020;-89.35583;,
 -10.23333;-79.67187;58.99606;,
 -20.88934;-95.14555;-6.58231;,
 1.32782;-26.71733;0.33206;,
 -38.38310;-3.21678;101.87038;,
 -111.38561;241.09842;-72.64382;,
 -74.39471;4.72596;-114.48667;,
 -76.30976;43.41801;-117.16531;,
 -102.12527;-144.39606;128.44579;,
 -83.73540;-92.08136;109.72555;,
 -123.19558;-92.08136;117.97602;,
 -46.55177;-99.69762;92.90099;,
 -89.66355;-163.29102;-93.03614;,
 -75.65115;-111.75033;-95.80568;,
 -130.36505;-227.60651;-70.29246;,
 -113.97292;-166.30911;-91.11345;,
 -111.18742;-111.75033;-103.73351;,
 -81.76940;103.38060;-116.71455;,
 -108.13145;181.14166;-92.74959;,
 -115.23101;112.02914;-118.51603;,
 -115.45429;162.29228;-114.52259;,
 -131.14828;43.41801;-128.07339;,
 -74.18623;-79.60815;-100.55684;,
 -111.56469;-67.64877;-105.95687;,
 -127.05927;4.72596;-124.96220;;
 
 102;
 3;0,1,2;,
 4;3,4,5,6;,
 4;4,7,8,5;,
 4;9,6,10,11;,
 4;6,5,12,10;,
 4;12,8,13,14;,
 4;15,13,16,17;,
 4;18,19,20,21;,
 3;22,23,24;,
 3;23,25,24;,
 3;25,26,24;,
 3;27,28,29;,
 3;27,30,31;,
 3;10,30,11;,
 3;10,12,14;,
 3;32,9,33;,
 3;1,4,3;,
 3;2,9,32;,
 3;9,3,6;,
 3;32,34,35;,
 3;2,35,0;,
 3;2,1,3;,
 3;25,23,36;,
 3;37,38,39;,
 3;40,41,28;,
 3;40,39,38;,
 3;42,43,44;,
 3;28,27,44;,
 3;29,34,32;,
 3;29,28,41;,
 3;15,10,14;,
 3;15,17,45;,
 3;13,15,14;,
 3;8,12,5;,
 3;3,9,2;,
 3;35,2,32;,
 3;9,11,33;,
 3;46,0,35;,
 3;41,47,48;,
 3;39,43,21;,
 3;21,37,39;,
 3;20,19,49;,
 3;20,26,25;,
 3;49,50,51;,
 3;49,26,20;,
 3;16,50,17;,
 3;50,52,45;,
 3;50,16,51;,
 3;45,17,50;,
 3;42,45,52;,
 3;43,39,40;,
 3;38,53,54;,
 3;22,24,55;,
 3;53,56,57;,
 3;53,38,37;,
 3;57,54,53;,
 3;29,32,33;,
 3;29,33,27;,
 3;30,27,33;,
 3;30,33,11;,
 3;15,31,30;,
 3;15,30,10;,
 3;42,44,27;,
 3;42,27,31;,
 3;43,40,28;,
 3;43,28,44;,
 3;36,23,37;,
 3;36,37,21;,
 3;20,25,36;,
 3;20,36,21;,
 3;41,48,58;,
 3;41,58,29;,
 3;34,29,58;,
 3;34,58,59;,
 3;34,59,46;,
 3;34,46,35;,
 3;58,60,61;,
 3;58,61,59;,
 3;48,62,60;,
 3;48,60,58;,
 3;47,63,64;,
 3;47,64,65;,
 3;40,63,47;,
 3;40,47,41;,
 3;48,47,65;,
 3;48,65,62;,
 3;53,22,55;,
 3;53,55,56;,
 3;23,22,53;,
 3;23,53,37;,
 3;63,54,57;,
 3;63,57,64;,
 3;38,54,63;,
 3;38,63,40;,
 3;18,21,43;,
 3;18,43,42;,
 3;19,18,42;,
 3;19,42,52;,
 3;49,19,52;,
 3;49,52,50;,
 3;45,42,31;,
 3;45,31,15;;
 
 MeshMaterialList {
  2;
  102;
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
  0;;
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   1.000000;
   0.100000;0.100000;0.100000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "data\\TEXTURE\\rock000.png";
   }
  }
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
 }
 MeshNormals {
  67;
  0.278711;0.602886;-0.747562;,
  0.400756;0.916183;-0.002091;,
  0.423465;0.766424;0.482982;,
  0.667847;0.697123;0.260767;,
  0.608056;0.628873;0.484547;,
  0.581072;0.635418;0.508526;,
  0.506480;0.721597;-0.471991;,
  0.894878;0.384057;-0.227361;,
  0.873179;0.369554;0.317786;,
  0.509768;0.528643;0.678729;,
  0.353355;0.376149;-0.856535;,
  0.700949;0.380052;0.603515;,
  0.552732;0.404459;0.728629;,
  0.417497;0.270990;-0.867330;,
  0.518740;0.355544;-0.777494;,
  0.958390;0.284190;0.026912;,
  0.895223;0.293879;0.334980;,
  0.508543;0.229659;0.829844;,
  0.381653;0.342695;0.858429;,
  0.108715;0.061150;-0.992190;,
  0.494547;0.193547;-0.847327;,
  0.898107;0.174638;-0.403617;,
  0.990423;0.124723;-0.059210;,
  0.869101;0.285960;0.403597;,
  0.738235;0.182972;0.649254;,
  0.563672;0.081814;0.821937;,
  0.346426;0.094335;0.933322;,
  0.195064;0.016463;-0.980652;,
  0.318292;0.004007;-0.947984;,
  0.900428;-0.106860;-0.421675;,
  0.830388;0.174576;0.529130;,
  0.560086;0.185389;0.807424;,
  0.193919;-0.109364;-0.974903;,
  0.415094;-0.128031;-0.900725;,
  0.730003;0.011297;-0.683351;,
  0.985092;-0.150502;-0.083325;,
  0.922622;-0.179396;0.341447;,
  0.423710;0.054547;0.904154;,
  0.136542;-0.141083;-0.980537;,
  0.415297;-0.122541;-0.901395;,
  0.759007;-0.143333;-0.635110;,
  0.962543;-0.213988;-0.166495;,
  0.558001;0.011615;0.829759;,
  0.350425;-0.045185;0.935500;,
  0.318684;-0.080175;0.944464;,
  0.411678;-0.155367;-0.897988;,
  0.629354;-0.245846;-0.737207;,
  0.852341;-0.257878;-0.454988;,
  0.550778;0.005096;0.834636;,
  0.204628;-0.017142;0.978690;,
  0.128448;-0.134793;-0.982513;,
  0.444203;-0.384997;-0.808987;,
  0.912020;-0.379281;0.156094;,
  0.695473;-0.390940;0.602896;,
  0.466092;-0.353077;0.811230;,
  0.917559;-0.392336;0.064485;,
  0.581843;-0.658174;0.477772;,
  0.364399;-0.673283;0.643354;,
  -0.042851;-0.267998;-0.962466;,
  0.751194;-0.551808;-0.362237;,
  0.282824;-0.600886;-0.747627;,
  0.704978;-0.690024;-0.163931;,
  0.779890;-0.590468;0.207654;,
  0.549772;-0.834769;0.030194;,
  0.525276;-0.677502;-0.514856;,
  0.554121;-0.684002;0.474438;,
  0.197164;0.268131;0.942991;;
  102;
  3;1,2,3;,
  4;4,5,12,11;,
  4;5,9,18,12;,
  4;8,11,24,16;,
  4;11,12,17,24;,
  4;17,18,26,25;,
  4;31,26,44,37;,
  4;52,53,56,55;,
  3;64,61,63;,
  3;61,65,63;,
  3;65,57,63;,
  3;22,29,21;,
  3;22,23,30;,
  3;24,23,16;,
  3;24,17,25;,
  3;7,8,15;,
  3;2,5,4;,
  3;3,8,7;,
  3;8,4,11;,
  3;7,14,6;,
  3;3,6,1;,
  3;3,2,4;,
  3;65,61,62;,
  3;59,46,47;,
  3;40,34,29;,
  3;40,47,46;,
  3;36,41,35;,
  3;29,22,35;,
  3;21,14,7;,
  3;21,29,34;,
  3;31,24,25;,
  3;31,37,42;,
  3;26,31,25;,
  3;18,17,12;,
  3;4,8,3;,
  3;6,3,7;,
  3;8,16,15;,
  3;0,1,6;,
  3;34,33,28;,
  3;47,41,55;,
  3;55,59,47;,
  3;56,53,54;,
  3;56,57,65;,
  3;66,43,49;,
  3;54,57,56;,
  3;44,43,37;,
  3;43,48,42;,
  3;43,44,49;,
  3;42,37,43;,
  3;36,42,48;,
  3;41,47,40;,
  3;46,51,45;,
  3;64,63,60;,
  3;51,58,50;,
  3;51,46,59;,
  3;50,45,51;,
  3;21,7,15;,
  3;21,15,22;,
  3;23,22,15;,
  3;23,15,16;,
  3;31,30,23;,
  3;31,23,24;,
  3;36,35,22;,
  3;36,22,30;,
  3;41,40,29;,
  3;41,29,35;,
  3;62,61,59;,
  3;62,59,55;,
  3;56,65,62;,
  3;56,62,55;,
  3;34,28,20;,
  3;34,20,21;,
  3;14,21,20;,
  3;14,20,10;,
  3;14,10,0;,
  3;14,0,6;,
  3;20,19,13;,
  3;20,13,10;,
  3;28,27,19;,
  3;28,19,20;,
  3;33,39,38;,
  3;33,38,32;,
  3;40,39,33;,
  3;40,33,34;,
  3;28,33,32;,
  3;28,32,27;,
  3;51,64,60;,
  3;51,60,58;,
  3;61,64,51;,
  3;61,51,59;,
  3;39,45,50;,
  3;39,50,38;,
  3;46,45,39;,
  3;46,39,40;,
  3;52,55,41;,
  3;52,41,36;,
  3;53,52,36;,
  3;53,36,48;,
  3;54,53,48;,
  3;54,48,43;,
  3;42,36,30;,
  3;42,30,31;;
 }
 MeshTextureCoords {
  66;
  0.173885;0.034376;,
  0.266942;0.034666;,
  0.233225;0.097646;,
  0.326504;0.104504;,
  0.428103;0.051745;,
  0.403830;0.147596;,
  0.365449;0.163695;,
  0.462763;0.098583;,
  0.464954;0.149785;,
  0.296476;0.142270;,
  0.358191;0.244152;,
  0.295068;0.210712;,
  0.406996;0.216275;,
  0.494197;0.434673;,
  0.413807;0.305955;,
  0.385672;0.389852;,
  0.477560;0.569488;,
  0.419939;0.479415;,
  0.297595;0.777817;,
  0.374247;0.768135;,
  0.351258;0.837892;,
  0.230307;0.772609;,
  0.089597;0.892906;,
  0.175587;0.902421;,
  0.212044;0.979984;,
  0.333739;0.881541;,
  0.407888;0.898647;,
  0.235166;0.428543;,
  0.190450;0.566656;,
  0.186093;0.277054;,
  0.291016;0.345803;,
  0.311592;0.455566;,
  0.161529;0.152292;,
  0.238267;0.204843;,
  0.112063;0.173083;,
  0.142182;0.108549;,
  0.263781;0.862157;,
  0.180251;0.868618;,
  0.122133;0.735267;,
  0.166912;0.762420;,
  0.139969;0.624060;,
  0.129113;0.444062;,
  0.303576;0.661309;,
  0.226989;0.705721;,
  0.237837;0.557055;,
  0.361313;0.507300;,
  0.061567;0.114828;,
  0.081920;0.489618;,
  0.078671;0.408147;,
  0.443195;0.773237;,
  0.410365;0.713489;,
  0.466823;0.723655;,
  0.359320;0.710365;,
  0.078257;0.799937;,
  0.091984;0.728187;,
  0.029464;0.888317;,
  0.047685;0.817355;,
  0.046279;0.738177;,
  0.073571;0.301120;,
  0.055005;0.174863;,
  0.036611;0.277912;,
  0.037462;0.214603;,
  0.016522;0.404546;,
  0.090346;0.671675;,
  0.044980;0.660713;,
  0.021592;0.489089;;
 }
}
