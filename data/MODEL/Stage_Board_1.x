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
 349;
 12.26425;3.49615;-0.01025;,
 -12.67765;1.31402;-0.01025;,
 -12.26425;-3.41115;-0.01025;,
 12.67765;-1.22902;-0.01025;,
 -12.67765;1.31402;-0.92025;,
 -12.26425;-3.41115;-0.92025;,
 12.26425;3.49615;-0.92025;,
 12.67765;-1.22902;-0.92025;,
 -3.26530;0.27630;-0.94075;,
 -6.78685;-0.03180;-0.94075;,
 -6.69664;-1.06286;-0.94075;,
 -3.17510;-0.75476;-0.94075;,
 -6.78685;-0.03180;-1.85075;,
 -6.69664;-1.06286;-1.85075;,
 -3.26530;0.27630;-1.85075;,
 -3.17510;-0.75476;-1.85075;,
 6.69664;1.14786;-0.94075;,
 3.17510;0.83976;-0.94075;,
 3.26530;-0.19130;-0.94075;,
 6.78685;0.11680;-0.94075;,
 3.17510;0.83976;-1.85075;,
 3.26530;-0.19130;-1.85075;,
 6.69664;1.14786;-1.85075;,
 6.78685;0.11680;-1.85075;,
 -5.00000;11.33774;-1.04075;,
 -5.25102;11.33774;-1.14473;,
 -5.25102;-0.32226;-1.14473;,
 -5.00000;-0.32226;-1.04075;,
 -5.35500;11.33774;-1.39575;,
 -5.35500;-0.32226;-1.39575;,
 -5.25102;11.33774;-1.64677;,
 -5.25102;-0.32226;-1.64677;,
 -5.00000;11.33774;-1.75075;,
 -5.00000;-0.32226;-1.75075;,
 -4.74898;11.33774;-1.64677;,
 -4.74898;-0.32226;-1.64677;,
 -4.64500;11.33774;-1.39575;,
 -4.64500;-0.32226;-1.39575;,
 -4.74898;11.33774;-1.14473;,
 -4.74898;-0.32226;-1.14473;,
 -5.00000;11.33774;-1.39575;,
 -5.00000;-0.32226;-1.39575;,
 5.00000;12.17843;-1.04075;,
 4.74898;12.17843;-1.14473;,
 4.74898;0.51843;-1.14473;,
 5.00000;0.51843;-1.04075;,
 4.64500;12.17843;-1.39575;,
 4.64500;0.51843;-1.39575;,
 4.74898;12.17843;-1.64677;,
 4.74898;0.51843;-1.64677;,
 5.00000;12.17843;-1.75075;,
 5.00000;0.51843;-1.75075;,
 5.25102;12.17843;-1.64677;,
 5.25102;0.51843;-1.64677;,
 5.35500;12.17843;-1.39575;,
 5.35500;0.51843;-1.39575;,
 5.25102;12.17843;-1.14473;,
 5.25102;0.51843;-1.14473;,
 5.00000;12.17843;-1.39575;,
 5.00000;0.51843;-1.39575;,
 15.48289;4.37169;-0.66703;,
 14.78647;3.33508;0.63813;,
 17.18516;2.18652;-0.05590;,
 16.10457;4.00930;-0.53918;,
 14.05702;4.82179;-0.87887;,
 13.32157;3.62911;0.55965;,
 13.18078;4.82584;-1.13534;,
 12.44534;3.63315;0.30317;,
 12.47245;4.58005;-1.30985;,
 11.77593;3.54729;-0.00180;,
 11.56933;4.10869;-1.77052;,
 10.91732;3.25878;-0.61164;,
 14.84236;1.74017;1.47977;,
 15.55305;1.74347;1.30928;,
 13.33863;1.87040;1.52888;,
 12.46239;1.87446;1.27241;,
 11.83191;1.94852;0.83694;,
 11.01781;1.84286;0.07795;,
 10.77381;8.09580;-0.82659;,
 12.31156;7.15280;-0.73137;,
 12.74263;9.13826;-2.58250;,
 11.17325;8.59524;-1.18038;,
 10.13557;6.84642;-0.26779;,
 11.86311;5.82420;-0.20961;,
 9.79113;6.00276;-0.21109;,
 11.51867;4.98056;-0.15291;,
 9.67766;5.25273;-0.34207;,
 11.21562;4.31071;-0.24214;,
 9.42567;4.25106;-0.77004;,
 10.74688;3.40074;-0.62238;,
 13.89314;6.77607;-1.51360;,
 13.85909;7.45891;-1.77192;,
 13.63407;5.36626;-1.03831;,
 13.28963;4.52262;-0.98160;,
 12.79700;3.93299;-1.02908;,
 12.11151;3.11472;-1.36158;,
 7.63104;4.71152;3.12304;,
 8.46404;5.93224;2.08432;,
 5.91328;6.87582;2.48169;,
 7.12488;5.22965;3.22055;,
 8.68476;3.75975;2.60889;,
 9.56003;5.14091;1.44313;,
 9.14696;3.19752;2.05763;,
 10.02222;4.57870;0.89188;,
 9.43852;2.88150;1.41910;,
 10.27486;4.10274;0.38383;,
 9.60566;2.36069;0.44400;,
 10.39748;3.39908;-0.44212;,
 8.27158;7.34304;0.97624;,
 7.67640;7.49547;1.37209;,
 9.40316;6.71114;0.20114;,
 9.86535;6.14891;-0.35012;,
 10.07906;5.51302;-0.72769;,
 10.15717;4.62651;-1.40447;,
 12.35009;0.84959;3.37273;,
 10.94891;1.97673;3.54395;,
 10.36577;-0.21376;5.09842;,
 12.06616;0.50589;3.95188;,
 12.61617;1.59730;2.08793;,
 11.02845;2.80801;2.30251;,
 12.46239;1.87446;1.27241;,
 10.95884;3.09348;1.43808;,
 12.24327;2.07100;0.54145;,
 10.84512;3.20149;0.71428;,
 11.75928;2.08051;-0.46635;,
 10.57788;3.11931;-0.34125;,
 9.23074;2.16933;4.05974;,
 9.38032;1.64221;4.54342;,
 9.11767;3.07745;2.85845;,
 9.04806;3.36293;1.99402;,
 9.12393;3.39073;1.22846;,
 9.07343;3.21684;0.12519;,
 -9.78451;-4.67681;4.02725;,
 -10.00855;-2.88842;3.59787;,
 -10.27936;-2.43528;3.26695;,
 -11.55833;-3.28368;3.39248;,
 -10.97809;-1.71541;2.46017;,
 -12.37218;-2.69854;2.60378;,
 -11.35267;-1.47773;1.82064;,
 -12.74676;-2.46089;1.96425;,
 -11.69401;-1.49177;1.26033;,
 -12.97448;-2.33740;1.38848;,
 -11.99880;-1.60709;0.36466;,
 -13.14935;-2.29550;0.47515;,
 -11.61837;-4.83666;3.81674;,
 -12.14901;-4.69796;3.52114;,
 -13.07500;-4.25313;2.74525;,
 -13.44957;-4.01545;2.10572;,
 -13.56366;-3.75446;1.51451;,
 -13.60863;-3.55534;0.58352;,
 -10.42399;2.30561;1.80482;,
 -12.24970;2.04552;1.62598;,
 -12.74000;1.68685;1.48753;,
 -11.94150;0.55664;2.16309;,
 -13.55376;0.74863;1.14862;,
 -12.62474;-0.48465;1.88795;,
 -13.85939;0.13648;0.77757;,
 -12.93036;-1.09679;1.51691;,
 -13.90642;-0.42869;0.44739;,
 -13.11057;-1.55875;1.12611;,
 -13.87852;-1.12781;-0.19980;,
 -10.36314;0.58313;2.48470;,
 -10.54895;-0.01157;2.48485;,
 -11.09639;-1.15624;2.26715;,
 -11.40201;-1.76838;1.89611;,
 -11.71537;-2.12711;1.44470;,
 -11.99195;-2.59021;0.65891;,
 -17.74095;1.38888;0.43602;,
 -17.57735;-0.31933;1.13469;,
 -17.25325;-0.84318;1.22815;,
 -16.03730;0.08882;1.38338;,
 -16.38288;-1.79082;1.26992;,
 -15.05327;-0.72211;1.41373;,
 -15.81733;-2.28192;1.05831;,
 -14.48772;-1.21319;1.20212;,
 -15.28376;-2.52295;0.76190;,
 -14.06778;-1.59174;0.92116;,
 -14.63949;-2.85809;0.14463;,
 -13.55343;-2.08411;0.32155;,
 -15.96754;1.62892;0.91583;,
 -15.38360;1.41951;0.97396;,
 -14.28597;0.74691;0.98485;,
 -13.85939;0.13648;0.77757;,
 -13.41411;-0.26026;0.50772;,
 -13.02966;-0.90985;-0.07424;,
 -17.15983;-5.71918;2.68275;,
 -15.39455;-5.37889;3.13089;,
 -14.85098;-5.09092;3.03187;,
 -15.71246;-3.87712;2.63708;,
 -13.86556;-4.38049;2.60579;,
 -14.85907;-3.06163;2.15387;,
 -13.44957;-4.01545;2.10572;,
 -14.36248;-2.70291;1.67377;,
 -13.12969;-3.71166;1.59914;,
 -13.99005;-2.49603;1.20784;,
 -12.81812;-3.46300;0.73340;,
 -13.52625;-2.36538;0.41140;,
 -17.28111;-3.91651;2.27218;,
 -17.04204;-3.39251;2.03457;,
 -16.32294;-2.47562;1.48725;,
 -15.81733;-2.28192;1.05831;,
 -15.32074;-2.01324;0.60183;,
 -14.70468;-2.00060;-0.12531;,
 -10.69447;3.98842;3.78720;,
 -10.14456;4.25575;3.43946;,
 -10.48761;4.82949;3.40809;,
 -9.83705;4.40250;2.96558;,
 -9.84475;4.93444;2.96453;,
 -10.14021;5.32324;2.81924;,
 -9.46005;4.92177;2.55229;,
 -9.87616;5.47136;2.34576;,
 -9.07438;4.92039;1.89391;,
 -9.50630;5.51247;1.68062;,
 -9.46005;4.92177;2.55229;,
 -9.93803;4.40146;2.28889;,
 -9.44531;4.64168;1.69000;,
 -9.07438;4.92039;1.89391;,
 -9.07438;4.92039;1.89391;,
 -9.44531;4.64168;1.69000;,
 -9.03304;4.78123;1.21328;,
 -9.20888;5.41586;1.13751;,
 -7.36346;2.05466;-1.28457;,
 -8.15018;1.93981;-0.88540;,
 -7.74073;1.78169;-1.53869;,
 -7.77185;3.39371;-0.13453;,
 -8.18633;4.28202;0.77324;,
 -7.82825;2.94097;0.09535;,
 -7.60968;2.67049;-1.78569;,
 -8.19358;3.97108;-0.63409;,
 -8.24106;3.45797;0.79030;,
 -9.03304;4.78123;1.21328;,
 -9.27498;3.59959;1.25381;,
 -9.12571;3.07357;0.98528;,
 -8.18633;4.28202;0.77324;,
 -8.07870;2.20543;-0.30471;,
 -8.24106;3.45797;0.79030;,
 -8.07870;2.20543;-0.30471;,
 -9.12571;3.07357;0.98528;,
 -8.07870;2.20543;-0.30471;,
 -7.41575;1.78102;-2.13553;,
 -7.36346;2.05466;-1.28457;,
 -8.73833;4.91636;0.24311;,
 -8.18633;4.28202;0.77324;,
 -8.61389;4.38613;0.02927;,
 -8.61389;4.38613;0.02927;,
 -8.87383;5.38251;0.52536;,
 -9.20888;5.41586;1.13751;,
 -8.87383;5.38251;0.52536;,
 -9.03304;4.78123;1.21328;,
 -7.74073;1.78169;-1.53869;,
 -7.41575;1.78102;-2.13553;,
 -7.73150;2.76216;-1.02044;,
 -7.15945;1.22159;-2.09145;,
 -7.22199;0.91029;-2.66685;,
 -7.74073;1.78169;-1.53869;,
 -7.04336;1.99925;-2.42151;,
 -7.73150;2.76216;-1.02044;,
 -7.41575;1.78102;-2.13553;,
 -7.41575;1.78102;-2.13553;,
 -7.36346;2.05466;-1.28457;,
 -7.04336;1.99925;-2.42151;,
 -7.36346;2.05466;-1.28457;,
 -7.41575;1.78102;-2.13553;,
 -7.73150;2.76216;-1.02044;,
 -10.66969;4.25575;3.14837;,
 -10.14456;4.25575;3.43946;,
 -10.42580;4.93444;2.64245;,
 -10.43078;4.40250;2.63648;,
 -9.83705;4.40250;2.96558;,
 -10.28007;4.92177;2.09775;,
 -9.93803;4.40146;2.28889;,
 -9.92610;4.92039;1.42179;,
 -9.44531;4.64168;1.69000;,
 -9.92610;4.92039;1.42179;,
 -8.13725;2.05466;-1.71349;,
 -8.89612;3.39371;-0.75772;,
 -9.06118;2.94097;-0.58807;,
 -9.44632;4.28202;0.07482;,
 -9.43177;3.45797;0.13028;,
 -9.44632;4.28202;0.07482;,
 -8.58919;2.20543;-0.58768;,
 -9.43177;3.45797;0.13028;,
 -8.58919;2.20543;-0.58768;,
 -8.58919;2.20543;-0.58768;,
 -8.13725;2.05466;-1.71349;,
 -9.44632;4.28202;0.07482;,
 -9.45003;4.63539;0.68899;,
 -9.45003;4.63539;0.68899;,
 -9.03304;4.78123;1.21328;,
 -7.74073;1.78169;-1.53869;,
 -8.34755;2.61630;-1.27337;,
 -7.73150;2.76216;-1.02044;,
 -8.34755;2.61630;-1.27337;,
 -8.13725;2.05466;-1.71349;,
 -8.13725;2.05466;-1.71349;,
 -8.34755;2.61630;-1.27337;,
 -8.58919;2.20543;-0.58768;,
 -8.90103;1.76897;-0.22047;,
 -8.61431;1.60662;-0.62408;,
 -8.15018;1.93981;-0.88540;,
 -10.48761;4.82949;3.40809;,
 -9.84475;4.93444;2.96453;,
 -9.84475;4.93444;2.96453;,
 -9.83705;4.40250;2.96558;,
 -9.83705;4.40250;2.96558;,
 -9.93803;4.40146;2.28889;,
 -9.50630;5.51247;1.68062;,
 -9.07438;4.92039;1.89391;,
 -9.20888;5.41586;1.13751;,
 -7.74073;1.78169;-1.53869;,
 -7.73150;2.76216;-1.02044;,
 -7.04336;1.99925;-2.42151;,
 -7.41575;1.78102;-2.13553;,
 -8.07870;2.20543;-0.30471;,
 -8.03452;1.60662;-0.30270;,
 -8.22486;1.76897;0.15434;,
 -8.03452;1.60662;-0.30270;,
 -8.07870;2.20543;-0.30471;,
 -8.22486;1.76897;0.15434;,
 -8.07870;2.20543;-0.30471;,
 -8.15018;1.93981;-0.88540;,
 -8.03452;1.60662;-0.30270;,
 -8.15018;1.93981;-0.88540;,
 -8.07870;2.20543;-0.30471;,
 -8.03452;1.60662;-0.30270;,
 -10.14021;5.32324;2.81924;,
 -10.43078;4.40250;2.63648;,
 -9.93803;4.40146;2.28889;,
 -9.50630;5.51247;1.68062;,
 -9.20888;5.41586;1.13751;,
 -8.34755;2.61630;-1.27337;,
 -8.58919;2.20543;-0.58768;,
 -8.90103;1.76897;-0.22047;,
 -8.61431;1.60662;-0.62408;,
 -8.61431;1.60662;-0.62408;,
 -8.90103;1.76897;-0.22047;,
 -8.58919;2.20543;-0.58768;,
 -8.07870;2.20543;-0.30471;,
 -8.15018;1.93981;-0.88540;,
 -8.58919;2.20543;-0.58768;,
 -8.58919;2.20543;-0.58768;,
 -8.15018;1.93981;-0.88540;,
 -8.07870;2.20543;-0.30471;,
 -8.15018;1.93981;-0.88540;,
 -8.61431;1.60662;-0.62408;,
 -8.58919;2.20543;-0.58768;,
 -8.58919;2.20543;-0.58768;,
 -8.61431;1.60662;-0.62408;,
 -8.90103;1.76897;-0.22047;;
 
 261;
 4;0,1,2,3;,
 4;1,4,5,2;,
 4;4,6,7,5;,
 4;6,0,3,7;,
 4;6,4,1,0;,
 4;3,2,5,7;,
 4;8,9,10,11;,
 4;9,12,13,10;,
 4;12,14,15,13;,
 4;14,8,11,15;,
 4;14,12,9,8;,
 4;11,10,13,15;,
 4;16,17,18,19;,
 4;17,20,21,18;,
 4;20,22,23,21;,
 4;22,16,19,23;,
 4;22,20,17,16;,
 4;19,18,21,23;,
 4;24,25,26,27;,
 4;25,28,29,26;,
 4;28,30,31,29;,
 4;30,32,33,31;,
 4;32,34,35,33;,
 4;34,36,37,35;,
 4;36,38,39,37;,
 4;38,24,27,39;,
 3;40,25,24;,
 3;40,28,25;,
 3;40,30,28;,
 3;40,32,30;,
 3;40,34,32;,
 3;40,36,34;,
 3;40,38,36;,
 3;40,24,38;,
 3;41,27,26;,
 3;41,26,29;,
 3;41,29,31;,
 3;41,31,33;,
 3;41,33,35;,
 3;41,35,37;,
 3;41,37,39;,
 3;41,39,27;,
 4;42,43,44,45;,
 4;43,46,47,44;,
 4;46,48,49,47;,
 4;48,50,51,49;,
 4;50,52,53,51;,
 4;52,54,55,53;,
 4;54,56,57,55;,
 4;56,42,45,57;,
 3;58,43,42;,
 3;58,46,43;,
 3;58,48,46;,
 3;58,50,48;,
 3;58,52,50;,
 3;58,54,52;,
 3;58,56,54;,
 3;58,42,56;,
 3;59,45,44;,
 3;59,44,47;,
 3;59,47,49;,
 3;59,49,51;,
 3;59,51,53;,
 3;59,53,55;,
 3;59,55,57;,
 3;59,57,45;,
 4;60,61,62,63;,
 4;64,65,61,60;,
 4;66,67,65,64;,
 4;68,69,67,66;,
 4;70,71,69,68;,
 4;61,72,73,62;,
 4;65,74,72,61;,
 4;67,75,74,65;,
 4;69,76,75,67;,
 4;71,77,76,69;,
 4;78,79,80,81;,
 4;82,83,79,78;,
 4;84,85,83,82;,
 4;86,87,85,84;,
 4;88,89,87,86;,
 4;79,90,91,80;,
 4;83,92,90,79;,
 4;85,93,92,83;,
 4;87,94,93,85;,
 4;89,95,94,87;,
 4;96,97,98,99;,
 4;100,101,97,96;,
 4;102,103,101,100;,
 4;104,105,103,102;,
 4;106,107,105,104;,
 4;97,108,109,98;,
 4;101,110,108,97;,
 4;103,111,110,101;,
 4;105,112,111,103;,
 4;107,113,112,105;,
 4;114,115,116,117;,
 4;118,119,115,114;,
 4;120,121,119,118;,
 4;122,123,121,120;,
 4;124,125,123,122;,
 4;115,126,127,116;,
 4;119,128,126,115;,
 4;121,129,128,119;,
 4;123,130,129,121;,
 4;125,131,130,123;,
 4;132,133,134,135;,
 4;135,134,136,137;,
 4;137,136,138,139;,
 4;139,138,140,141;,
 4;141,140,142,143;,
 4;144,132,135,145;,
 4;145,135,137,146;,
 4;146,137,139,147;,
 4;147,139,141,148;,
 4;148,141,143,149;,
 4;150,151,152,153;,
 4;153,152,154,155;,
 4;155,154,156,157;,
 4;157,156,158,159;,
 4;159,158,160,143;,
 4;161,150,153,162;,
 4;162,153,155,163;,
 4;163,155,157,164;,
 4;164,157,159,165;,
 4;165,159,143,166;,
 4;167,168,169,170;,
 4;170,169,171,172;,
 4;172,171,173,174;,
 4;174,173,175,176;,
 4;176,175,177,178;,
 4;179,167,170,180;,
 4;180,170,172,181;,
 4;181,172,174,182;,
 4;182,174,176,183;,
 4;183,176,178,184;,
 4;185,186,187,188;,
 4;188,187,189,190;,
 4;190,189,191,192;,
 4;192,191,193,194;,
 4;194,193,195,196;,
 4;197,185,188,198;,
 4;198,188,190,199;,
 4;199,190,192,200;,
 4;200,192,194,201;,
 4;201,194,196,202;,
 3;203,204,205;,
 4;205,204,206,207;,
 4;208,207,209,210;,
 4;210,209,211,212;,
 4;213,214,215,216;,
 4;217,218,219,220;,
 3;221,222,223;,
 3;224,225,226;,
 3;224,227,228;,
 3;224,226,221;,
 3;229,230,231;,
 3;229,231,232;,
 3;233,229,226;,
 3;233,230,229;,
 3;234,235,232;,
 3;236,222,221;,
 3;236,237,222;,
 3;238,226,235;,
 3;238,221,226;,
 3;239,221,223;,
 3;240,227,224;,
 3;240,239,227;,
 3;241,242,243;,
 3;244,224,228;,
 3;244,242,224;,
 3;245,246,242;,
 3;247,242,241;,
 3;248,225,246;,
 3;249,250,251;,
 3;252,249,253;,
 3;252,250,249;,
 3;252,254,250;,
 3;252,253,254;,
 3;255,256,257;,
 3;255,258,259;,
 3;255,259,256;,
 3;260,261,262;,
 3;260,263,261;,
 3;255,256,257;,
 3;255,258,259;,
 3;255,259,256;,
 3;260,261,262;,
 3;260,263,261;,
 3;203,205,264;,
 3;203,264,265;,
 4;205,266,267,264;,
 4;264,267,268,265;,
 4;208,210,269,266;,
 4;266,269,270,267;,
 4;210,212,271,269;,
 4;269,271,272,270;,
 4;273,220,219,218;,
 3;274,223,222;,
 3;275,276,277;,
 3;275,228,227;,
 3;275,274,276;,
 3;278,231,230;,
 3;278,232,231;,
 3;279,276,278;,
 3;279,278,230;,
 3;280,232,281;,
 3;282,274,222;,
 3;282,222,237;,
 3;283,281,276;,
 3;283,276,274;,
 3;239,223,274;,
 3;284,275,227;,
 3;284,227,239;,
 3;241,243,285;,
 3;244,228,275;,
 3;244,275,285;,
 3;245,285,246;,
 3;247,241,285;,
 3;286,246,277;,
 3;286,277,230;,
 3;287,288,246;,
 4;289,250,290,291;,
 3;249,290,250;,
 3;252,253,249;,
 3;252,249,250;,
 3;252,250,254;,
 3;252,254,253;,
 3;255,257,292;,
 3;255,293,258;,
 3;255,292,293;,
 3;260,262,294;,
 3;260,294,295;,
 3;255,257,292;,
 3;255,293,258;,
 3;255,292,293;,
 3;260,262,294;,
 3;260,294,295;,
 4;296,297,298,299;,
 3;300,301,208;,
 3;302,303,213;,
 3;304,305,213;,
 3;306,307,308;,
 3;309,310,250;,
 3;311,312,256;,
 3;311,312,256;,
 3;313,314,315;,
 3;316,317,318;,
 3;319,320,321;,
 3;322,323,324;,
 3;300,325,266;,
 3;326,327,268;,
 3;328,329,273;,
 3;311,330,257;,
 3;311,330,257;,
 3;331,332,333;,
 3;334,335,336;,
 3;337,338,339;,
 3;340,341,342;,
 3;343,344,345;,
 3;346,347,348;;
 
 MeshMaterialList {
  3;
  261;
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
  0,
  1,
  1,
  1,
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
  1,
  1,
  1,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
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
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "data\\TEXTURE\\timber.jpg";
   }
  }
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "data\\TEXTURE\\shaft.jpg";
   }
  }
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "data\\TEXTURE\\leaf.jpg";
   }
  }
 }
 MeshNormals {
  265;
  0.000000;0.000000;1.000000;,
  -0.996195;-0.087156;0.000000;,
  0.000000;0.000000;-1.000000;,
  -0.996194;-0.087159;0.000000;,
  -0.996195;-0.087150;0.000000;,
  0.000000;1.000000;0.000000;,
  -0.707107;0.000000;0.707107;,
  -1.000000;0.000000;-0.000000;,
  -0.707107;0.000000;-0.707107;,
  0.707107;0.000000;-0.707107;,
  1.000000;0.000000;-0.000000;,
  0.707107;0.000000;0.707107;,
  0.000000;-1.000000;0.000000;,
  0.996195;0.087156;0.000000;,
  -0.087156;0.996195;0.000000;,
  0.087156;-0.996195;0.000000;,
  0.996195;0.087150;0.000000;,
  -0.087157;0.996195;0.000000;,
  0.087157;-0.996195;0.000000;,
  0.996194;0.087159;0.000000;,
  -0.087157;0.996195;0.000000;,
  0.087157;-0.996195;0.000000;,
  0.355646;0.581868;0.731400;,
  0.242196;0.666764;0.704817;,
  -0.021136;0.777723;0.628252;,
  -0.262387;0.808690;0.526473;,
  -0.444985;0.804623;0.393153;,
  -0.527804;0.789847;0.312353;,
  0.346040;0.541042;0.766505;,
  0.222364;0.583196;0.781304;,
  -0.059037;0.639592;0.766444;,
  -0.316136;0.644729;0.695976;,
  -0.519694;0.612447;0.595673;,
  -0.615409;0.582757;0.530722;,
  0.335398;0.498597;0.799318;,
  0.199590;0.491913;0.847458;,
  -0.094599;0.476126;0.874274;,
  -0.351390;0.443048;0.824763;,
  -0.550995;0.369115;0.748438;,
  -0.642553;0.318414;0.696949;,
  0.329623;0.507868;0.795876;,
  0.243662;0.421841;0.873315;,
  0.079715;0.198419;0.976870;,
  -0.057179;-0.025933;0.998027;,
  -0.191599;-0.203767;0.960088;,
  -0.262034;-0.288143;0.921038;,
  0.379537;0.487727;0.786177;,
  0.349599;0.380490;0.856159;,
  0.263678;0.125247;0.956445;,
  0.164339;-0.121163;0.978934;,
  0.069417;-0.326613;0.942605;,
  0.018273;-0.427350;0.903902;,
  0.428316;0.466126;0.774127;,
  0.450913;0.334107;0.827677;,
  0.437193;0.047115;0.898133;,
  0.376243;-0.209304;0.902570;,
  0.324635;-0.422179;0.846391;,
  0.296785;-0.524572;0.797962;,
  0.419491;0.463080;0.780759;,
  0.517585;0.357478;0.777377;,
  0.701460;0.139410;0.698941;,
  0.822509;-0.053611;0.566220;,
  0.869771;-0.231986;0.435524;,
  0.873965;-0.323193;0.362949;,
  0.430259;0.505468;0.747917;,
  0.542467;0.447852;0.710744;,
  0.753095;0.296050;0.587540;,
  0.893135;0.133105;0.429643;,
  0.962992;-0.014853;0.269120;,
  0.979453;-0.092000;0.179464;,
  0.439740;0.546344;0.712837;,
  0.560175;0.532304;0.634710;,
  0.774897;0.440962;0.452865;,
  0.911509;0.312034;0.267930;,
  0.975777;0.203521;0.080237;,
  0.988716;0.148232;-0.021653;,
  0.445525;0.537077;0.716280;,
  0.516124;0.602398;0.608878;,
  0.607499;0.719912;0.335666;,
  0.624852;0.772256;-0.114810;,
  0.608194;0.754794;-0.245736;,
  0.396763;0.558786;0.728243;,
  0.415231;0.650560;0.635889;,
  0.434068;0.811855;0.390481;,
  0.419165;0.895858;0.147443;,
  0.378381;0.923062;-0.069166;,
  0.345768;0.918106;-0.193713;,
  0.346814;0.578823;0.738027;,
  0.308845;0.690116;0.654488;,
  0.243099;0.869977;0.429003;,
  0.183874;0.964388;0.190121;,
  0.100145;0.994815;-0.017715;,
  0.049373;0.991220;-0.122660;,
  -0.209667;0.347086;0.914096;,
  -0.263687;0.462426;0.846541;,
  -0.386319;0.650503;0.653915;,
  -0.482779;0.760357;0.434491;,
  -0.518760;0.825844;0.221063;,
  -0.521969;0.846319;0.106270;,
  -0.244725;0.305546;0.920191;,
  -0.338809;0.376742;0.862133;,
  -0.522361;0.508480;0.684534;,
  -0.654289;0.593312;0.468920;,
  -0.730763;0.632631;0.256445;,
  -0.755537;0.640005;0.139849;,
  -0.279052;0.263091;0.923533;,
  -0.409451;0.286074;0.866321;,
  -0.637708;0.346315;0.688036;,
  -0.787521;0.391556;0.475915;,
  -0.881726;0.386575;0.270405;,
  -0.914876;0.370812;0.159688;,
  -0.279889;0.273988;0.920105;,
  -0.402657;0.203649;0.892409;,
  -0.611136;0.019717;0.791280;,
  -0.742773;-0.159951;0.650156;,
  -0.827073;-0.287031;0.483285;,
  -0.857030;-0.342313;0.385125;,
  -0.239585;0.242841;0.940014;,
  -0.319394;0.138355;0.937467;,
  -0.473237;-0.096383;0.875647;,
  -0.581213;-0.307355;0.753475;,
  -0.629995;-0.465812;0.621390;,
  -0.198564;0.210968;0.957113;,
  -0.231906;0.071230;0.970127;,
  -0.316593;-0.208663;0.925326;,
  -0.385648;-0.436778;0.812712;,
  -0.377005;-0.602440;0.703515;,
  -0.349377;-0.673810;0.651088;,
  -0.209732;0.209445;0.955063;,
  -0.147394;0.083074;0.985583;,
  0.018229;-0.162049;0.986614;,
  0.179725;-0.358209;0.916179;,
  0.288927;-0.520863;0.803258;,
  0.334671;-0.597782;0.728458;,
  -0.175302;0.251816;0.951766;,
  -0.075009;0.172381;0.982170;,
  0.158385;-0.011733;0.987308;,
  0.346310;-0.169166;0.922742;,
  0.485256;-0.303630;0.819961;,
  0.558564;-0.378631;0.738001;,
  -0.140347;0.293433;0.945621;,
  -0.001632;0.259408;0.965766;,
  0.292321;0.139625;0.946073;,
  0.640262;-0.059474;0.765851;,
  0.727579;-0.122281;0.675038;,
  -0.139496;0.282538;0.949059;,
  -0.008420;0.341849;0.939717;,
  0.258296;0.470233;0.843898;,
  0.591203;0.583356;0.556934;,
  0.669699;0.591008;0.449681;,
  -0.180436;0.314522;0.931944;,
  -0.094422;0.410772;0.906835;,
  0.109806;0.612186;0.783052;,
  0.259111;0.725259;0.637856;,
  0.382058;0.785076;0.487531;,
  0.462181;0.809217;0.362707;,
  -0.220837;0.345564;0.912040;,
  -0.179176;0.474263;0.861957;,
  -0.044429;0.726487;0.685743;,
  0.137984;0.917031;0.374185;,
  0.209257;0.947926;0.240099;,
  0.627920;0.772357;0.095814;,
  -0.639246;-0.538419;0.549064;,
  0.490350;0.029788;0.871016;,
  0.445646;0.550822;0.705688;,
  0.055024;0.848810;0.525828;,
  0.406567;0.290458;0.866220;,
  0.494608;0.372633;0.785180;,
  0.532109;0.264444;0.804319;,
  0.492966;0.607298;0.623036;,
  0.615731;0.378711;0.690979;,
  0.694093;0.123406;0.709229;,
  0.620286;0.636070;0.458977;,
  0.693063;0.450890;0.562460;,
  0.478558;-0.876310;0.055348;,
  0.693393;0.634752;0.341023;,
  0.778433;-0.593355;0.204872;,
  0.839297;0.305582;0.449668;,
  0.651327;-0.046740;0.757356;,
  0.791837;0.568239;-0.223828;,
  0.832567;0.239095;0.499665;,
  0.377508;-0.201859;0.903737;,
  0.817395;0.575540;-0.024863;,
  0.825809;-0.215287;0.521239;,
  -0.181322;-0.942839;0.279600;,
  0.922335;0.369759;-0.112144;,
  0.506681;-0.791802;0.341062;,
  0.228711;-0.842235;0.488192;,
  0.077956;-0.741356;0.666569;,
  0.917420;0.395506;0.043767;,
  0.516018;-0.275414;0.811093;,
  0.256403;-0.775194;0.577348;,
  0.699094;-0.373829;0.609524;,
  0.845516;0.533444;0.023248;,
  0.831228;0.405586;-0.380210;,
  0.885710;-0.078503;0.457553;,
  -0.331459;-0.814273;0.476544;,
  0.645006;-0.336940;0.685886;,
  -0.929451;-0.069788;-0.362286;,
  -0.991464;-0.073510;-0.107682;,
  -0.968014;0.244320;-0.057072;,
  -0.915658;0.314567;-0.250237;,
  -0.926836;-0.047893;-0.372400;,
  -0.711555;0.644083;-0.280796;,
  -0.651878;0.643314;-0.401501;,
  -0.859027;0.279715;-0.428757;,
  -0.879650;-0.215292;-0.424106;,
  -0.644332;-0.619834;-0.447931;,
  -0.513289;0.398568;-0.760051;,
  -0.971802;-0.233559;0.032434;,
  -0.780245;-0.614882;-0.114622;,
  -0.879316;0.410473;-0.241485;,
  0.723860;0.621529;-0.299546;,
  0.740998;-0.071663;0.667673;,
  0.679178;-0.729857;0.077633;,
  0.951477;0.284649;0.116906;,
  0.883085;0.454654;0.115979;,
  0.773841;0.511828;-0.373098;,
  0.804033;0.402819;0.437341;,
  0.764804;0.643640;0.028311;,
  -0.804033;-0.402819;-0.437341;,
  -0.777373;-0.490801;-0.393454;,
  -0.756294;-0.113932;-0.644235;,
  0.331459;0.814273;-0.476544;,
  -0.645006;0.336940;-0.685886;,
  -0.595575;0.650722;-0.471011;,
  -0.608807;0.765372;-0.208712;,
  -0.895561;-0.378630;-0.233686;,
  -0.885710;0.078503;-0.457553;,
  0.895561;0.378630;0.233686;,
  -0.959796;0.270252;0.075863;,
  -0.930218;0.365486;-0.033390;,
  0.205640;-0.905584;-0.370985;,
  0.164598;-0.940602;-0.296944;,
  0.061124;-0.992020;-0.110271;,
  0.317594;-0.940681;-0.119388;,
  -0.779920;0.617946;-0.099331;,
  -0.663181;-0.555645;-0.501448;,
  -0.380183;-0.682269;-0.624475;,
  -0.269985;-0.571292;-0.775070;,
  -0.361439;0.593732;-0.718919;,
  -0.406804;0.375588;-0.832733;,
  -0.532361;0.296116;-0.793036;,
  -0.986159;-0.076253;0.147226;,
  -0.925750;0.026243;0.377225;,
  -0.878858;-0.465294;0.105407;,
  -0.152367;0.408777;-0.899825;,
  -0.213344;0.556478;-0.803005;,
  -0.109766;0.502384;-0.857649;,
  -0.493893;0.536933;-0.683939;,
  -0.751300;0.540488;-0.378709;,
  -0.586363;-0.528955;0.613502;,
  -0.306283;-0.825912;0.473349;,
  -0.757221;-0.133468;0.639377;,
  -0.647570;0.123303;-0.751964;,
  0.532979;-0.140265;0.834421;,
  0.566907;-0.257681;0.782443;,
  0.944275;-0.000422;0.329156;,
  0.991464;0.073510;0.107682;,
  0.929451;0.069788;0.362286;,
  -0.985381;-0.072183;0.154318;,
  -0.720408;0.072143;-0.689789;,
  -0.799775;0.069786;-0.596229;,
  0.237297;0.872029;-0.428085;,
  -0.528876;0.073572;-0.845504;;
  261;
  4;0,0,0,0;,
  4;1,1,1,1;,
  4;2,2,2,2;,
  4;13,13,13,13;,
  4;14,14,14,14;,
  4;15,15,15,15;,
  4;0,0,0,0;,
  4;3,3,3,3;,
  4;2,2,2,2;,
  4;16,16,16,16;,
  4;17,17,17,17;,
  4;18,18,18,18;,
  4;0,0,0,0;,
  4;4,4,4,4;,
  4;2,2,2,2;,
  4;19,19,19,19;,
  4;20,20,20,20;,
  4;21,21,21,21;,
  4;0,6,6,0;,
  4;6,7,7,6;,
  4;7,8,8,7;,
  4;8,2,2,8;,
  4;2,9,9,2;,
  4;9,10,10,9;,
  4;10,11,11,10;,
  4;11,0,0,11;,
  3;5,5,5;,
  3;5,5,5;,
  3;5,5,5;,
  3;5,5,5;,
  3;5,5,5;,
  3;5,5,5;,
  3;5,5,5;,
  3;5,5,5;,
  3;12,12,12;,
  3;12,12,12;,
  3;12,12,12;,
  3;12,12,12;,
  3;12,12,12;,
  3;12,12,12;,
  3;12,12,12;,
  3;12,12,12;,
  4;0,6,6,0;,
  4;6,7,7,6;,
  4;7,8,8,7;,
  4;8,2,2,8;,
  4;2,9,9,2;,
  4;9,10,10,9;,
  4;10,11,11,10;,
  4;11,0,0,11;,
  3;5,5,5;,
  3;5,5,5;,
  3;5,5,5;,
  3;5,5,5;,
  3;5,5,5;,
  3;5,5,5;,
  3;5,5,5;,
  3;5,5,5;,
  3;12,12,12;,
  3;12,12,12;,
  3;12,12,12;,
  3;12,12,12;,
  3;12,12,12;,
  3;12,12,12;,
  3;12,12,12;,
  3;12,12,12;,
  4;23,29,28,22;,
  4;24,30,29,23;,
  4;25,31,30,24;,
  4;26,32,31,25;,
  4;27,33,32,26;,
  4;29,35,34,28;,
  4;30,36,35,29;,
  4;31,37,36,30;,
  4;32,38,37,31;,
  4;33,39,38,32;,
  4;41,47,46,40;,
  4;42,48,47,41;,
  4;43,49,48,42;,
  4;44,50,49,43;,
  4;45,51,50,44;,
  4;47,53,52,46;,
  4;48,54,53,47;,
  4;49,55,54,48;,
  4;50,56,55,49;,
  4;51,57,56,50;,
  4;59,65,64,58;,
  4;60,66,65,59;,
  4;61,67,66,60;,
  4;62,68,67,61;,
  4;63,69,68,62;,
  4;65,71,70,64;,
  4;66,72,71,65;,
  4;67,73,72,66;,
  4;68,74,73,67;,
  4;69,75,74,68;,
  4;77,82,81,76;,
  4;78,83,82,77;,
  4;161,84,83,78;,
  4;79,85,84,161;,
  4;80,86,85,79;,
  4;82,88,87,81;,
  4;83,89,88,82;,
  4;84,90,89,83;,
  4;85,91,90,84;,
  4;86,92,91,85;,
  4;99,93,94,100;,
  4;100,94,95,101;,
  4;101,95,96,102;,
  4;102,96,97,103;,
  4;103,97,98,104;,
  4;105,99,100,106;,
  4;106,100,101,107;,
  4;107,101,102,108;,
  4;108,102,103,109;,
  4;109,103,104,110;,
  4;117,111,112,118;,
  4;118,112,113,119;,
  4;119,113,114,120;,
  4;120,114,115,121;,
  4;121,115,116,162;,
  4;122,117,118,123;,
  4;123,118,119,124;,
  4;124,119,120,125;,
  4;125,120,121,126;,
  4;126,121,162,127;,
  4;134,128,129,135;,
  4;135,129,130,136;,
  4;136,130,131,137;,
  4;137,131,132,138;,
  4;138,132,133,139;,
  4;140,134,135,141;,
  4;141,135,136,142;,
  4;142,136,137,163;,
  4;163,137,138,143;,
  4;143,138,139,144;,
  4;150,145,146,151;,
  4;151,146,147,152;,
  4;152,147,164,153;,
  4;153,164,148,154;,
  4;154,148,149,155;,
  4;156,150,151,157;,
  4;157,151,152,158;,
  4;158,152,153,165;,
  4;165,153,154,159;,
  4;159,154,155,160;,
  3;166,168,167;,
  4;167,168,171,170;,
  4;169,170,173,172;,
  4;172,173,175,175;,
  4;214,174,176,176;,
  4;176,176,178,177;,
  3;186,184,187;,
  3;189,180,183;,
  3;189,185,182;,
  3;189,183,215;,
  3;190,178,181;,
  3;190,181,188;,
  3;180,190,183;,
  3;180,178,190;,
  3;191,190,188;,
  3;191,184,186;,
  3;191,188,184;,
  3;191,183,190;,
  3;191,186,183;,
  3;192,186,187;,
  3;215,185,189;,
  3;215,216,185;,
  3;194,217,179;,
  3;179,189,182;,
  3;179,217,189;,
  3;193,177,180;,
  3;193,180,194;,
  3;178,180,177;,
  3;187,192,195;,
  3;196,187,196;,
  3;218,216,219;,
  3;220,221,222;,
  3;223,223,223;,
  3;197,195,192;,
  3;224,225,226;,
  3;227,227,228;,
  3;197,186,192;,
  3;229,195,215;,
  3;197,195,192;,
  3;224,225,226;,
  3;227,227,228;,
  3;197,186,192;,
  3;229,195,215;,
  3;230,231,200;,
  3;232,233,233;,
  4;231,201,202,200;,
  4;233,234,235,233;,
  4;236,203,203,201;,
  4;201,237,237,202;,
  4;203,204,204,203;,
  4;237,238,238,237;,
  4;238,239,239,238;,
  3;207,221,184;,
  3;208,206,205;,
  3;208,240,241;,
  3;208,242,206;,
  3;209,243,244;,
  3;209,245,243;,
  3;205,206,209;,
  3;205,209,244;,
  3;210,245,209;,
  3;210,207,184;,
  3;210,184,188;,
  3;210,209,206;,
  3;210,206,207;,
  3;222,221,207;,
  3;242,208,241;,
  3;242,241,225;,
  3;246,247,248;,
  3;247,240,208;,
  3;247,208,248;,
  3;249,205,250;,
  3;249,246,205;,
  3;211,250,205;,
  3;251,252,252;,
  3;251,244,253;,
  4;219,216,212,212;,
  3;221,254,222;,
  3;223,223,219;,
  3;220,221,222;,
  3;218,216,219;,
  3;196,187,196;,
  3;224,222,254;,
  3;197,255,192;,
  3;197,256,255;,
  3;224,222,207;,
  3;224,242,254;,
  3;224,222,254;,
  3;197,255,192;,
  3;197,256,255;,
  3;224,222,207;,
  3;224,242,254;,
  4;213,213,213,257;,
  3;167,170,169;,
  3;170,171,173;,
  3;235,174,214;,
  3;175,175,177;,
  3;221,228,222;,
  3;224,222,228;,
  3;224,222,228;,
  3;199,199,198;,
  3;258,258,259;,
  3;199,260,199;,
  3;257,258,258;,
  3;231,236,201;,
  3;234,174,235;,
  3;204,250,204;,
  3;197,256,192;,
  3;197,256,192;,
  3;213,213,213;,
  3;261,262,210;,
  3;263,263,263;,
  3;210,184,191;,
  3;264,261,264;,
  3;210,261,262;;
 }
 MeshTextureCoords {
  349;
  0.743300;-0.136840;,
  0.743300;1.115020;,
  0.506140;1.115020;,
  0.506140;-0.136840;,
  0.743300;1.115020;,
  0.506140;1.115020;,
  0.743300;-0.136840;,
  0.506140;-0.136840;,
  0.650590;0.650710;,
  0.650590;0.827460;,
  0.598840;0.827460;,
  0.598840;0.650710;,
  0.650590;0.827460;,
  0.598840;0.827460;,
  0.650590;0.650710;,
  0.598840;0.650710;,
  0.650600;0.150710;,
  0.650600;0.327460;,
  0.598850;0.327460;,
  0.598850;0.150710;,
  0.650600;0.327460;,
  0.598850;0.327460;,
  0.650600;0.150710;,
  0.598850;0.150710;,
  1.209120;0.688920;,
  1.210210;0.701420;,
  0.629430;0.752230;,
  0.628340;0.739730;,
  1.210670;0.706600;,
  0.629890;0.757410;,
  1.210210;0.701420;,
  0.629430;0.752230;,
  1.209120;0.688920;,
  0.628340;0.739730;,
  1.208030;0.676420;,
  0.627240;0.727220;,
  1.207570;0.671240;,
  0.626790;0.722050;,
  1.208030;0.676420;,
  0.627240;0.727220;,
  1.209120;0.688920;,
  0.628340;0.739730;,
  1.207420;0.187160;,
  1.208510;0.199660;,
  0.627730;0.250470;,
  0.626640;0.237970;,
  1.208970;0.204840;,
  0.628190;0.255650;,
  1.208510;0.199660;,
  0.627730;0.250470;,
  1.207420;0.187160;,
  0.626640;0.237970;,
  1.206330;0.174660;,
  0.625540;0.225460;,
  1.205870;0.169480;,
  0.625090;0.220280;,
  1.206330;0.174660;,
  0.625540;0.225460;,
  1.207420;0.187160;,
  0.626640;0.237970;,
  0.200000;0.000000;,
  0.200000;0.500000;,
  0.000000;0.500000;,
  0.000000;0.000000;,
  0.400000;0.000000;,
  0.400000;0.500000;,
  0.600000;0.000000;,
  0.600000;0.500000;,
  0.800000;0.000000;,
  0.800000;0.500000;,
  1.000000;0.000000;,
  1.000000;0.500000;,
  0.200000;1.000000;,
  0.000000;1.000000;,
  0.400000;1.000000;,
  0.600000;1.000000;,
  0.800000;1.000000;,
  1.000000;1.000000;,
  0.200000;0.000000;,
  0.200000;0.500000;,
  0.000000;0.500000;,
  0.000000;0.000000;,
  0.400000;0.000000;,
  0.400000;0.500000;,
  0.600000;0.000000;,
  0.600000;0.500000;,
  0.800000;0.000000;,
  0.800000;0.500000;,
  1.000000;0.000000;,
  1.000000;0.500000;,
  0.200000;1.000000;,
  0.000000;1.000000;,
  0.400000;1.000000;,
  0.600000;1.000000;,
  0.800000;1.000000;,
  1.000000;1.000000;,
  0.200000;0.000000;,
  0.200000;0.500000;,
  0.000000;0.500000;,
  0.000000;0.000000;,
  0.400000;0.000000;,
  0.400000;0.500000;,
  0.600000;0.000000;,
  0.600000;0.500000;,
  0.800000;0.000000;,
  0.800000;0.500000;,
  1.000000;0.000000;,
  1.000000;0.500000;,
  0.200000;1.000000;,
  0.000000;1.000000;,
  0.400000;1.000000;,
  0.600000;1.000000;,
  0.800000;1.000000;,
  1.000000;1.000000;,
  0.200000;0.000000;,
  0.200000;0.500000;,
  0.000000;0.500000;,
  0.000000;0.000000;,
  0.400000;0.000000;,
  0.400000;0.500000;,
  0.600000;0.000000;,
  0.600000;0.500000;,
  0.800000;0.000000;,
  0.800000;0.500000;,
  1.000000;0.000000;,
  1.000000;0.500000;,
  0.200000;1.000000;,
  0.000000;1.000000;,
  0.400000;1.000000;,
  0.600000;1.000000;,
  0.800000;1.000000;,
  1.000000;1.000000;,
  0.000000;0.500000;,
  0.000000;0.000000;,
  0.200000;0.000000;,
  0.200000;0.500000;,
  0.400000;0.000000;,
  0.400000;0.500000;,
  0.600000;0.000000;,
  0.600000;0.500000;,
  0.800000;0.000000;,
  0.800000;0.500000;,
  1.000000;0.000000;,
  1.000000;0.500000;,
  0.000000;1.000000;,
  0.200000;1.000000;,
  0.400000;1.000000;,
  0.600000;1.000000;,
  0.800000;1.000000;,
  1.000000;1.000000;,
  0.000000;0.500000;,
  0.000000;0.000000;,
  0.200000;0.000000;,
  0.200000;0.500000;,
  0.400000;0.000000;,
  0.400000;0.500000;,
  0.600000;0.000000;,
  0.600000;0.500000;,
  0.800000;0.000000;,
  0.800000;0.500000;,
  1.000000;0.000000;,
  0.000000;1.000000;,
  0.200000;1.000000;,
  0.400000;1.000000;,
  0.600000;1.000000;,
  0.800000;1.000000;,
  1.000000;1.000000;,
  0.000000;0.500000;,
  0.000000;0.000000;,
  0.200000;0.000000;,
  0.200000;0.500000;,
  0.400000;0.000000;,
  0.400000;0.500000;,
  0.600000;0.000000;,
  0.600000;0.500000;,
  0.800000;0.000000;,
  0.800000;0.500000;,
  1.000000;0.000000;,
  1.000000;0.500000;,
  0.000000;1.000000;,
  0.200000;1.000000;,
  0.400000;1.000000;,
  0.600000;1.000000;,
  0.800000;1.000000;,
  1.000000;1.000000;,
  0.000000;0.500000;,
  0.000000;0.000000;,
  0.200000;0.000000;,
  0.200000;0.500000;,
  0.400000;0.000000;,
  0.400000;0.500000;,
  0.600000;0.000000;,
  0.600000;0.500000;,
  0.800000;0.000000;,
  0.800000;0.500000;,
  1.000000;0.000000;,
  1.000000;0.500000;,
  0.000000;1.000000;,
  0.200000;1.000000;,
  0.400000;1.000000;,
  0.600000;1.000000;,
  0.800000;1.000000;,
  1.000000;1.000000;,
  0.146260;-0.750590;,
  0.202460;-0.770620;,
  0.206610;-0.805400;,
  0.255840;-0.777840;,
  0.262390;-0.834810;,
  0.263610;-0.852840;,
  0.312440;-0.825230;,
  0.315840;-0.864410;,
  0.382050;-0.813600;,
  0.386400;-0.860220;,
  0.307680;-0.784610;,
  0.304400;-0.769560;,
  0.376820;-0.783790;,
  0.377530;-0.775690;,
  0.400990;-0.811570;,
  0.396030;-0.773300;,
  0.435730;-0.789450;,
  0.437600;-0.807620;,
  0.674470;-0.452610;,
  0.605230;-0.451310;,
  0.673530;-0.422800;,
  0.582280;-0.614030;,
  0.503090;-0.724060;,
  0.555780;-0.569010;,
  0.710420;-0.513720;,
  0.609500;-0.672320;,
  0.488650;-0.636500;,
  0.405980;-0.769060;,
  0.406320;-0.665290;,
  0.427640;-0.604380;,
  0.498020;-0.697890;,
  0.566250;-0.486860;,
  0.496630;-0.617750;,
  0.568590;-0.477770;,
  0.449780;-0.586790;,
  0.582830;-0.496970;,
  0.735500;-0.412510;,
  0.689480;-0.470840;,
  0.527420;-0.788990;,
  0.516520;-0.719080;,
  0.543200;-0.728530;,
  0.565790;-0.700000;,
  0.483870;-0.811300;,
  0.442010;-0.858730;,
  0.504360;-0.803950;,
  0.457780;-0.769990;,
  0.702020;-0.410470;,
  0.708160;-0.409020;,
  0.637190;-0.520040;,
  0.736710;-0.351350;,
  0.775100;-0.310920;,
  0.717420;-0.428620;,
  0.781030;-0.414030;,
  0.652710;-0.508410;,
  0.776430;-0.428160;,
  0.770860;-0.411730;,
  0.641820;-0.476300;,
  0.782550;-0.415210;,
  0.643400;-0.477520;,
  0.772440;-0.412950;,
  0.654290;-0.509620;,
  0.202460;-0.770620;,
  0.200560;-0.763030;,
  0.262390;-0.834810;,
  0.255840;-0.777840;,
  0.253640;-0.770070;,
  0.312440;-0.825230;,
  0.307680;-0.784610;,
  0.382050;-0.813600;,
  0.377530;-0.775690;,
  0.400990;-0.811570;,
  0.674470;-0.452610;,
  0.582280;-0.614030;,
  0.555780;-0.569010;,
  0.503090;-0.724060;,
  0.488650;-0.636500;,
  0.498020;-0.697890;,
  0.566250;-0.486860;,
  0.496630;-0.617750;,
  0.568590;-0.477770;,
  0.582830;-0.496970;,
  0.689480;-0.470840;,
  0.516520;-0.719080;,
  0.457780;-0.769990;,
  0.425850;-0.791210;,
  0.431230;-0.773330;,
  0.712910;-0.419510;,
  0.637190;-0.520040;,
  0.645750;-0.534490;,
  0.652710;-0.508410;,
  0.641820;-0.476300;,
  0.643400;-0.477520;,
  0.654290;-0.509620;,
  0.544510;-0.449850;,
  0.545630;-0.442460;,
  0.556770;-0.440810;,
  0.559050;-0.447690;,
  0.200975;-0.811090;,
  0.234500;-0.820105;,
  0.287415;-0.830020;,
  0.284140;-0.801535;,
  0.254740;-0.773955;,
  0.279020;-0.769815;,
  0.423710;-0.866045;,
  0.423080;-0.854205;,
  0.438950;-0.825640;,
  0.679330;-0.477000;,
  0.641470;-0.527265;,
  0.716150;-0.469500;,
  0.715360;-0.468890;,
  0.522800;-0.434425;,
  0.524285;-0.431645;,
  0.494690;-0.414640;,
  0.522350;-0.428520;,
  0.521235;-0.431585;,
  0.487780;-0.419160;,
  0.557910;-0.444250;,
  0.563505;-0.443420;,
  0.565330;-0.439540;,
  0.567785;-0.442785;,
  0.568420;-0.442690;,
  0.566300;-0.439390;,
  0.235110;-0.829120;,
  0.281760;-0.781225;,
  0.306040;-0.777085;,
  0.421665;-0.838200;,
  0.438275;-0.816630;,
  0.653500;-0.509015;,
  0.522800;-0.434425;,
  0.493015;-0.416000;,
  0.557230;-0.445930;,
  0.522350;-0.428520;,
  0.488895;-0.416095;,
  0.552460;-0.450140;,
  0.559425;-0.463310;,
  0.566695;-0.455070;,
  0.569390;-0.468990;,
  0.544700;-0.454075;,
  0.551780;-0.448770;,
  0.560640;-0.455970;,
  0.568905;-0.442615;,
  0.562710;-0.439925;,
  0.562130;-0.447230;,
  0.541985;-0.450220;,
  0.542185;-0.446580;,
  0.539420;-0.443390;;
 }
}