#include <GL/glut.h>
#include <iostream>

using namespace std;

enum BAR_STATE{
    BAR_STOP,
    BAR_MOVIMENT_UP,
    BAR_MOVIMENT_DOWN
};

enum DIFICULT_SETTING{
    EASY,
    NORMAL,
    HARD,
    VERY_HARD,
    HYPER_HARD,
};

// Tamanho da janela
const int width = 800;
const int height = 600;

// Posições e dimensões das barras
float barWidth = 10.0f;
float barHeight = 100.0f;
float bar1X = 20.0f;
float bar2X = width-20;
float bar1Y = height / 2 - barHeight / 2;
float bar2Y = height / 2 - barHeight / 2;

// Estado das barras
BAR_STATE bar1State = BAR_STOP;
BAR_STATE bar2State = BAR_STOP;

// Posição e velocidade da bola
float ballX = width / 2;
float ballY = height / 2;
float ballSize = 20.0f;

// Velocidades
float ballXSpeed = 5.0f;
float ballYSpeed = 5.0f;
float ballSpeed = 5.0f;
float ballDirectionX = 1.0f;
float ballDirectionY = 1.0f;

float barSpeed = 10.0f;

float bar1XSpeed = 0.0f;
float bar2XSpeed = 0.0f;

float bar1YSpeed = 10.0f;
float bar2YSpeed = 10.0f;

// Placar
int score1 = 0;
int score2 = 0;

//Renderiza um objeto quadricular na tela e atribui a ele velocidade
void renderGameObjetcs(float x, float y, float width, float height, float* speedX,float* speedY){
    glPushMatrix();
    glTranslatef(*speedX,*speedY,0);
        glBegin(GL_QUADS);
            glVertex2f(x - width, y - height);
            glVertex2f(x + width, y - height);
            glVertex2f(x + width, y + height);
            glVertex2f(x - width, y + height);
        glEnd();
    glPopMatrix();
    glFlush();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    // Desenha o placar
    glRasterPos2f(width / 2 - 50, height - 50);
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, '0' + score1);
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, ' ');
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, '-');
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, ' ');
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, '0' + score2);

    // Espaço para desenhar as barras e a bola
    //barra 1
    renderGameObjetcs(bar1X,bar1Y,barWidth,barHeight,&bar1XSpeed,&bar1YSpeed);
    //barra 2
    renderGameObjetcs(bar2X,bar2Y,barWidth,barHeight,&bar2XSpeed,&bar2YSpeed);
    //bola
    renderGameObjetcs(ballX,ballY,ballSize,ballSize,&ballXSpeed,&ballYSpeed);

    glFlush();
    glutSwapBuffers();
}

void resetBall(){
    ballXSpeed = ballSpeed;
    ballYSpeed = ballSpeed;
    ballDirectionY = -1;
    ballDirectionX = -1; 
}

void resetBar(int bar){
    if(bar == 1){
        bar1XSpeed = 0.0f;
        bar1YSpeed = 10.0f;
    }
    else if(bar ==2){
        bar2XSpeed = 0.0f;
        bar2YSpeed = 10.0f;
    }
}

void resetGame(){
    score1 = 0;
    score2 = 0;
    
    resetBall();
    resetBar(1);
    resetBar(2);
}

void setDificulty(DIFICULT_SETTING dificulty){
    switch (dificulty)
    {
    case EASY:
        ballSpeed = 4;
        barHeight = 120;
        break;
    case NORMAL:
        ballSpeed = 5;
        barHeight = 100;
        break;
    case HARD:
        ballSpeed = 6;
        barHeight = 80;
        break;
    case VERY_HARD:
        ballSpeed = 7;
        barHeight = 80;
        break;
    case HYPER_HARD:
        ballSpeed = 8;
        barHeight = 50;
        break;
    }

    resetGame();
}

void makeScore(int player){
    resetBall();
    if(player == 1){
        score1 += 1;
    }
    else if(player == 2){
        score2 += 1;
    }
    if(score1 >= 9 || score2 >= 9){
        resetGame();
    }
}

void update(int value) {
    //Movimenta barra 1 baseada em seu estado
    switch (bar1State)
    {
        case BAR_MOVIMENT_UP:
            if(bar1Y + barHeight + bar1YSpeed < height){
                bar1YSpeed += barSpeed;
            }
            break;
        case BAR_MOVIMENT_DOWN:
            if(bar1Y - barHeight + bar1YSpeed > 0){    
                bar1YSpeed -= barSpeed;
            }
            break;
        case BAR_STOP:
            bar1YSpeed = bar1YSpeed;
            break;
    }
    //Movimenta barra 2 baseada em seu estado
    switch (bar2State)
    {
        case BAR_MOVIMENT_UP:
            if(bar2Y + barHeight + bar2YSpeed < height){
                bar2YSpeed += barSpeed;
            }
            break;
        case BAR_MOVIMENT_DOWN:
            if(bar2Y - barHeight + bar2YSpeed > 0){  
                bar2YSpeed -= barSpeed;
            }
            break;
        case BAR_STOP:
            bar2YSpeed = bar2YSpeed;
            break;
        
    }

    //Movimenta a bola em X e Y
    ballXSpeed += ballDirectionX * ballSpeed;
    ballYSpeed += ballDirectionY * ballSpeed;

    //Marca pontos qdo a bola passa pela direita ou esquerda
    if (ballX + ballSize + ballXSpeed > width+20) {
        makeScore(1);
    }
    else if(ballX + ballXSpeed - ballSize  < -20){
        makeScore(2);
    }

    //Logica de rebater a bola
    if (ballY + ballSize + ballYSpeed> height || ballY + ballYSpeed - ballSize  < 0) {
        ballDirectionY = -ballDirectionY; // Inverter a direção vertical
    }
    if(ballX + ballSize + ballXSpeed - 20 <= bar1X + barWidth && ballY + ballSize + ballYSpeed +20 >= bar1Y - barHeight + bar1YSpeed && ballY + ballSize + ballYSpeed -20 <= bar1Y + barHeight + bar1YSpeed ){
        ballDirectionX = 1;
    }
    if(ballX + ballSize + ballXSpeed + 20 >= bar2X + barWidth && ballY + ballSize + ballYSpeed +20 >= bar2Y - barHeight + bar2YSpeed && ballY + ballSize + ballYSpeed -20 <= bar2Y + barHeight + bar2YSpeed ){
        ballDirectionX = -1;
    }

    glutPostRedisplay();
    glutTimerFunc(16, update, 0); // Aproximadamente 60 FPS
}

void handleKeysDown(unsigned char key, int x, int y) {
    switch(key){
        case 's':
            bar1State = BAR_MOVIMENT_UP;
            break;
        case 'd':
            bar1State = BAR_MOVIMENT_DOWN;
            break;
        case 'k':
            bar2State = BAR_MOVIMENT_UP;
            break;
        case 'l':
            bar2State = BAR_MOVIMENT_DOWN;
            break;
        case '1':
            setDificulty(EASY);
            break;
        case '2':
            setDificulty(NORMAL);
            break;
        case '3':
            setDificulty(HARD);
            break;
        case '4':
            setDificulty(VERY_HARD);
            break;
        case '5':
            setDificulty(HYPER_HARD);
            break;
    }
}

void handleKeysUp(unsigned char key, int x, int y) {
    bar1State = BAR_STOP;
    bar2State = BAR_STOP;
}

// Função para evitar o redimensionamento da janela
void reshape(int w, int h) {
    glutReshapeWindow(width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    // aqui a origem do sistema de coordenadas é colocada na parte inferior à esquerda
    gluOrtho2D(0.0, width, 0.0, height); 
    glutPostRedisplay();
}

void Init(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(width, height);
    glutCreateWindow("TP3 - Jogo Pong");

}

int main(int argc, char** argv) {
    Init(argc, argv);
    glutDisplayFunc(display);
    glutTimerFunc(16, update, 0);
    glutKeyboardFunc(handleKeysDown);
    glutKeyboardUpFunc(handleKeysUp);
    glutReshapeFunc(reshape);

    glutMainLoop();
    return 0;
}