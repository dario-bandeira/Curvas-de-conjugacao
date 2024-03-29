double a,b,c,d,e,f,g,h,i,j,x,y,pid,queda,dist,pas;
int linha, coluna;

double mapf(double x, double in_min, double in_max, double out_min, double out_max){
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

double m[27][12] = {

//PID //Pás    //DIST --------------------------------------------------//
{0.00, 0.00,   0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00}, // 0
{0.50, 0.00,   0.73, 0.71, 0.69, 0.66, 0.64, 0.62, 0.60, 0.59, 0.58, 0.56}, // 1
{1.00, 0.00,   1.47, 1.42, 1.38, 1.34, 1.30, 1.25, 1.22, 1.18, 1.15, 1.12}, // 2
{1.50, 0.00,   2.20, 2.13, 2.07, 2.00, 1.94, 1.87, 1.82, 1.77, 1.72, 1.68}, // 3
{2.00, 0.00,   2.94, 2.85, 2.76, 2.67, 2.58, 2.49, 2.43, 2.37, 2.31, 2.25}, // 4
{2.51, 0.00,   3.68, 3.49, 3.37, 3.23, 3.10, 3.10, 3.04, 2.97, 2.91, 2.82}, // 5
{2.89, 0.50,   4.22, 4.00, 3.86, 3.69, 3.58, 3.54, 3.48, 3.39, 3.31, 3.24}, // 6
{3.27, 1.00,   4.74, 4.49, 4.32, 4.13, 4.00, 3.97, 3.88, 3.78, 3.69, 3.63}, // 7
{3.65, 1.50,   5.26, 4.97, 4.77, 4.54, 4.41, 4.39, 4.28, 4.15, 4.05, 3.99}, // 8
{4.04, 2.00,   5.77, 5.45, 5.22, 4.95, 4.80, 4.78, 4.66, 4.51, 4.38, 4.33}, // 9
{4.41, 2.50,   6.27, 5.92, 5.66, 5.34, 5.17, 5.12, 5.02, 4.85, 4.71, 4.65}, // 10
{4.79, 3.00,   6.74, 6.37, 6.08, 5.73, 5.53, 5.49, 5.36, 5.19, 5.04, 4.96}, // 11
{5.17, 3.50,   7.18, 6.77, 6.46, 6.09, 5.88, 5.81, 5.69, 5.51, 5.35, 5.25}, // 12
{5.55, 4.00,   7.56, 7.13, 6.82, 6.44, 6.20, 6.15, 6.00, 5.82, 5.64, 5.54}, // 13
{5.92, 4.50,   7.89, 7.46, 7.15, 6.75, 6.50, 6.44, 6.29, 6.10, 5.93, 5.82}, // 14
{6.30, 5.00,   8.17, 7.76, 7.44, 7.04, 6.79, 6.70, 6.57, 6.37, 6.19, 6.08}, // 15
{6.68, 5.50,   8.44, 8.03, 7.71, 7.31, 7.05, 7.00, 6.82, 6.62, 6.44, 6.33}, // 16
{7.07, 6.00,   8.70, 8.28, 7.96, 7.56, 7.29, 7.23, 7.06, 6.85, 6.67, 6.55}, // 17
{7.46, 6.50,   8.95, 8.52, 8.20, 7.79, 7.52, 7.45, 7.29, 7.02, 6.88, 6.76}, // 18
{7.85, 7.00,   9.21, 8.75, 8.43, 8.01, 7.74, 7.69, 7.50, 7.28, 7.09, 6.95}, // 19
{8.21, 7.50,   9.47, 8.98, 8.65, 8.23, 7.95, 7.90, 7.71, 7.49, 7.29, 7.15}, // 20
{8.58, 8.0
0,   9.72, 9.21, 8.87, 8.44, 8.16, 8.10, 7.92, 7.70, 7.49, 7.34}, // 21
{8.96, 8.50,   9.97, 9.44, 9.09, 8.66, 8.37, 8.31, 8.12, 7.80, 7.69, 7.54}, // 22
{9.35, 9.00,   10.0, 9.67, 9.31, 8.87, 8.58, 8.52, 8.33, 8.10, 7.91, 7.74}, // 23
{9.67, 9.50,   10.0, 9.91, 9.53, 9.08, 8.79, 8.74, 8.53, 8.30, 8.10, 7.95}, // 24
{10.0, 10.0,   10.0, 10.0, 9.76, 9.30, 8.99, 8.91, 8.73, 8.50, 8.29, 8.15}, // 25

{NULL, NULL,   10.0, 6.80, 4.40, 1.30,-1.07,-1.70,-3.44,-5.81,-8.18,-10.0}  // 26 Queda
//0    //1     //2   //3   //4   //5   //6   //7   //8   //9   //10  //11
};


void setup() {
  pinMode(A0, INPUT); //PID
  pinMode(A1, INPUT); //queda
  pinMode(7, OUTPUT); //pás
  pinMode(6, OUTPUT); //distribuidor
  Serial.begin(9600);
}

void loop() {

  pid = mapf(analogRead(A0),0,1023,0,10);
  queda = mapf(analogRead(A1),0,1023,10,-10);

  //localizando posição entre as linhas
  
  for (int i = 24; i >= 0; i--) {
    if (pid >= m[i][0]) {
      e = m[i][0];
      f = m[i+1][0];
      linha = i;
      break;
    }
  }

  //localizando posição entre as colunas

  for (int i = 10; i >= 2; i--) {
    if (queda <= m[26][i]) {
      g = m[26][i];
      h = m[26][i+1];
      coluna = i;
      break;
    }
  }
  
  a = m[linha][coluna];
  b = m[linha][coluna+1];
  c = m[linha+1][coluna];
  d = m[linha+1][coluna+1];

  x = (queda-h)/(g-h)*(a-b)+b;
  y = (queda-h)/(g-h)*(c-d)+d;
  dist = (pid-f)/(e-f)*(x-y)+y;

  pas = (pid - m[linha][0]) / (m[linha+1][0] - m[linha][0]) * (m[linha+1][1] - m[linha][1]) + m[linha][1];

  //mostrando na tela do computador
  Serial.print("PID: ");
  Serial.print(pid);
  Serial.print("  Queda: ");
  Serial.print(queda);
  Serial.print("  Pás: ");
  Serial.print(pas);
  Serial.print("  Distribuidor: ");
  Serial.println(dist);
  

  //saída no arduino
  analogWrite(7, mapf(pas,0,10,0,255));
  analogWrite(6, mapf(dist,0,10,0,255));

  delay(50);
}
