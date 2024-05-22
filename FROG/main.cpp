#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <iostream>
#include <string>
#include <stdio.h>
#include <fstream>
#include <cmath>
#include <vector>

using namespace std;



/*********TILE*CONST********/
const int LEVEL_WIDTH = 1000;
const int LEVEL_HEIGHT = 3800;

//Tile constants
const int TILE_WIDTH = 100;
const int TILE_HEIGHT = 100;
const int TOTAL_TILES = 380;
const int TOTAL_TILE_SPRITES = 11;

//The different tile sprites
const int TILE_NONE = 0;
const int TILE_LEAF = 1;
const int TILE_CENTER = 2;
const int TILE_TOP = 3;
const int TILE_TOPRIGHT = 4;
const int TILE_RIGHT = 5;
const int TILE_BOTTOMRIGHT = 6;
const int TILE_BOTTOM = 7;
const int TILE_BOTTOMLEFT = 8;
const int TILE_LEFT = 9;
const int TILE_TOPLEFT = 10;


/*********SDL*UTILS**********/
void initSDL(SDL_Window* &window, SDL_Renderer* &renderer, const char* WINDOW_TITLE, int SCREEN_WIDTH, int SCREEN_HEIGHT );
void logSDLError(std::ostream& os, const std::string &msg, bool fatal = false);
void quitSDL(SDL_Window* window, SDL_Renderer* renderer);


/************UI*************/
class UI{
public:
    //member variables
    SDL_Window* window;
    SDL_Renderer* render;
    const int       SCREEN_WIDTH        =                  800;
    const int       SCREEN_HEIGHT       =                  600;
    const char*     WINDOW_TITLE        =               "FROG";


    //constructor
    UI();
    ~UI();

    //member funtion
    inline void clearRender() { SDL_RenderClear(render); };
    inline void present()     { SDL_RenderPresent(render); };


private:

};
/*********VECTOR2D*******/
class Vector2D {
public:
    Vector2D(float _x=0, float _y=0);
    ~Vector2D();

    /***********************************************/
    inline void setXandY( float _x, float _y) { x = _x; y = _y; };

    /***********************************************/
    inline float getX() const { return x; };
    inline float getY() const { return y; };


    /***********************************************/
    Vector2D operator+(const Vector2D& Other) const;
    Vector2D operator- (const Vector2D& Other) const;
    Vector2D operator* (float Multiplier) const;
    Vector2D operator/ (float Divisor) const;
    Vector2D operator+= (const Vector2D& Other);
    Vector2D operator-= (const Vector2D& Other);
    Vector2D operator*= (float Multiplier);
    Vector2D operator/= (float Divisor);
    Vector2D operator- () const;
    bool operator==  (const Vector2D& other) const;
    void normalize();
    float length();


private:
    float x;
    float y;
};
/*********L*Texture******/
class LTexture{
	public:
		//Initializes variables
		LTexture();

		//Deallocates memory
		~LTexture();

		//Loads image at specified path
		bool loadFromFile( std::string path, SDL_Renderer* renderer );

		//Creates image from font string
		bool loadFromRenderedText( std::string textureText, SDL_Color textColor, SDL_Renderer* renderer, TTF_Font* font );

		//Deallocates texture
		void free();

		//Set color modulation
		void setColor( Uint8 red, Uint8 green, Uint8 blue );

		//Set blending
		void setBlendMode( SDL_BlendMode blending );

		//Set alpha modulation
		void setAlpha( Uint8 alpha );

		//Renders texture at given point
		void render( SDL_Renderer* renderer, int x, int y, SDL_Rect* clip = NULL,
                    double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE );

		//Gets image dimensions
		int getWidth();
		int getHeight();

	private:
		//The actual hardware texture
		SDL_Texture* mTexture;

		//Image dimensions
		int mWidth;
		int mHeight;
};
/*********TILE**********/
class Tile{
    public:
		//Initializes position and type
		Tile( int x, int y, int tileType );

		//Shows the tile
		void render( SDL_Renderer* renderer, SDL_Rect& camera, double rotation=0.0 );

		//Get the tile type
		int getType();

		//Get the collision box
		SDL_Rect getBox();

    private:
		//The attributes of the tile
		SDL_Rect mBox;

		//The tile type
		int mType;
};
/*********FROG**********/
class Frog{
public:

    static const int FROG_WIDTH     =   50;
    static const int FROG_HEIGHT    =   50;

    static const int FROG_VEL       =   12;

    inline bool getIsSpacePress() { return isSpacePress; };
    inline void setIsSpacePress(bool _isSpacePress) { isSpacePress = _isSpacePress; };

    inline SDL_Rect getBox() const { return mBox; };

    Frog( int initX, int initY, double initRotation );

    void handleEvent( SDL_Event& e, double currentRotation, int frame );

    void toMove( Tile* tiles[] );

    void setCamera( SDL_Rect& camera );

    void setBox( int& boxX, int& boxY );

    void setRotation( double _rotation );

    void setVtGetDirectionViaRotation();

    void render( SDL_Renderer* renderer, SDL_Rect& camera, SDL_Rect* currentClip, double currenRotation);

private:

    SDL_Rect mBox;

    int mVelX, mVelY;

    bool isSpacePress;

    Vector2D vtToGetDirection;

    double rotation;

};
/*******LEAF************/
class Leaf{
public:

    Leaf( Tile* _tileLeaf, double _angle=0, int _rotateSpeedCoeffcient=1 );
    ~Leaf();

    inline void setAngle( double _angle ){ mAngle = _angle; }
    inline double getAngle()const {return mAngle; }

    inline SDL_Rect getBox() const { return tileLeaf->getBox(); }

    inline Tile* getTile() const { return tileLeaf; }

    void setFrogCenter( Frog& _frog );

    void rotation();

    inline bool getIsFrogOn() { return isFrogOn; }
    inline void setIsFrogOn( bool _isCollid ) { isFrogOn = _isCollid; }

    inline bool getWasFrogOn() { return wasFrogOn; };
    inline void setWasFrogOn( bool _wasFrogOn ) { wasFrogOn = _wasFrogOn; };


private:

    Tile* tileLeaf;
    double mAngle;
    int rotateSpeedCoeffcient;
    const double ROTATION_SPEED = 1.0;
    bool isFrogOn;
    bool wasFrogOn;
};


/********GAME*LOGIC*FUNCTIONS****/
bool loadMedia( Tile* tiles[], SDL_Renderer* renderer );
//Frees media and shuts down SDL
void close( Tile* tiles[] );
//Box collision detector
bool checkCollision( SDL_Rect a, SDL_Rect b );
//Checks collision box against set of tiles
bool touchesWall( SDL_Rect box, Tile* tiles[] );
//Sets tiles from tile map
bool setTiles( Tile *tiles[] );
//Sets listLeaf from tiles
void setListLeaf( Tile *tiles[], vector<Leaf*>& _listLeaf );
//close listLeaf
void closeListLeaf( vector<Leaf*>& _listLeaf );
//get last tile Leaf in vector
Leaf* getLastpLeaf( vector<Leaf*>& _listLeaf );
//check frog is leaf
bool checkIsFrogLive( const vector<Leaf*>& _listLeaf );
//render BG
void renderBG( LTexture& _bg );


/*********GAME*VARIABLES**********/
UI ui;
//FROG HANDLE
Frog frog(0,0,0.0);
LTexture FrogTexture;
const int JUMP_ANIMATION_FRAME = 20;
SDL_Rect FrogClips[ JUMP_ANIMATION_FRAME ];
double currentRotation = 0.0;
double rotation = 0.0;
//TILE HANDLE
LTexture gTileTexture;
SDL_Rect gTileClips[ TOTAL_TILE_SPRITES ];
Tile* tileSet[ TOTAL_TILES ];
vector<Leaf*> listLeaf;
//BACKGOUND HANDLE
LTexture gBGTexture;
LTexture sandTexture;
int scrollingOffsetBG = 0;
int scrollingOffsetSand = 0;
//SCORE HANDLE
TTF_Font* gFont = NULL;
LTexture gPromptTextTexture;
LTexture gScore;
int score = 0;
SDL_Color scoreColor = { 0, 0, 0, 0xFF };
//CAMERA HANDLE
SDL_Rect camera = { 0, 0, ui.SCREEN_WIDTH, ui.SCREEN_HEIGHT };


//GAME INIT
bool StartScreen();
bool guideScreen();
bool gameOverScreen(bool& quit);
bool gameWinnerScreen(bool& quit);
void ScoreInit();
void listLeafInit();
void FrogInit();
void FrogInitPlayAgain();
bool GameInit();
void GameHandleEvent(bool& playAgain, bool& quit, SDL_Event& e, int& frame);
void GameRender(int& frame, const int& listLeafSize);
void GameLoop(bool& playAgain);
void GameClose();
void GameRun();



/*********MAIN**************/
int main(int argc, char** argv)
{

    GameRun();

    return 0;
}


/*********SDL*UTILS**********/

void logSDLError(std::ostream& os, const std::string &msg, bool fatal){
    os << msg << " Error: " << SDL_GetError() << std::endl;
    if (fatal) {
        SDL_Quit();
        exit(1);
    }
}

void initSDL(SDL_Window* &window, SDL_Renderer* &renderer, const char* WINDOW_TITLE, int SCREEN_WIDTH, int SCREEN_HEIGHT ){

    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
        logSDLError(std::cout, "SDL_Init", true);
     //Initialize SDL_ttf
    if( TTF_Init() == -1 ){
        logSDLError(std::cout, "TTF_Init", true);
    }


    window = SDL_CreateWindow(WINDOW_TITLE, SDL_WINDOWPOS_CENTERED,
       SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    //window = SDL_CreateWindow(WINDOW_TITLE.c_str(), SDL_WINDOWPOS_CENTERED,
      // SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_FULLSCREEN_DESKTOP);
    if (window == nullptr) logSDLError(std::cout, "CreateWindow", true);

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED |
                                              SDL_RENDERER_PRESENTVSYNC);
    //renderer = SDL_CreateSoftwareRenderer(SDL_GetWindowSurface(window));

    if (renderer == nullptr) logSDLError(std::cout, "CreateRenderer", true);

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
    SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);
}

void quitSDL(SDL_Window* window, SDL_Renderer* renderer){
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

/************UI*************/
UI::UI(){
    initSDL(window, render, WINDOW_TITLE, SCREEN_WIDTH, SCREEN_HEIGHT);
}

UI::~UI(){
    quitSDL(window, render);
}

/*********L*Texture******/
LTexture::LTexture(){
	//Initialize
	mTexture = NULL;
	mWidth = 0;
	mHeight = 0;
}

LTexture::~LTexture(){
	//Deallocate
	free();
}

bool LTexture::loadFromFile( std::string path, SDL_Renderer* renderer ){
	//Get rid of preexisting texture
	free();

	//The final texture
	SDL_Texture* newTexture = NULL;

	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
	if( loadedSurface == NULL )
	{
		printf( "Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError() );
	}
	else
	{
		//Color key image
		SDL_SetColorKey( loadedSurface, SDL_TRUE, SDL_MapRGB( loadedSurface->format, 0, 0xFF, 0xFF ) );

		//Create texture from surface pixels
        newTexture = SDL_CreateTextureFromSurface( renderer, loadedSurface );
		if( newTexture == NULL )
		{
			printf( "Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
		}
		else
		{
			//Get image dimensions
			mWidth = loadedSurface->w;
			mHeight = loadedSurface->h;
		}

		//Get rid of old loaded surface
		SDL_FreeSurface( loadedSurface );
	}

	//Return success
	mTexture = newTexture;
	return mTexture != NULL;
}

bool LTexture::loadFromRenderedText( std::string textureText, SDL_Color textColor, SDL_Renderer* renderer, TTF_Font* font ){
	//Get rid of preexisting texture
	free();

	//Render text surface
	SDL_Surface* textSurface = TTF_RenderText_Solid( font, textureText.c_str(), textColor );
	if( textSurface != NULL )
	{
		//Create texture from surface pixels
        mTexture = SDL_CreateTextureFromSurface( renderer, textSurface );
		if( mTexture == NULL )
		{
			printf( "Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError() );
		}
		else
		{
			//Get image dimensions
			mWidth = textSurface->w;
			mHeight = textSurface->h;
		}

		//Get rid of old surface
		SDL_FreeSurface( textSurface );
	}
	else
	{
		printf( "Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError() );
	}


	//Return success
	return mTexture != NULL;
}

void LTexture::free(){
	//Free texture if it exists
	if( mTexture != NULL )
	{
		SDL_DestroyTexture( mTexture );
		mTexture = NULL;
		mWidth = 0;
		mHeight = 0;
	}
}

void LTexture::setColor( Uint8 red, Uint8 green, Uint8 blue ){
	//Modulate texture rgb
	SDL_SetTextureColorMod( mTexture, red, green, blue );
}

void LTexture::setBlendMode( SDL_BlendMode blending ){
	//Set blending function
	SDL_SetTextureBlendMode( mTexture, blending );
}

void LTexture::setAlpha( Uint8 alpha ){
	//Modulate texture alpha
	SDL_SetTextureAlphaMod( mTexture, alpha );
}

void LTexture::render( SDL_Renderer* renderer, int x, int y, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip ){
	//Set rendering space and render to screen
	SDL_Rect renderQuad = { x, y, mWidth, mHeight };

	//Set clip rendering dimensions
	if( clip != NULL ){
		renderQuad.w = clip->w;
		renderQuad.h = clip->h;
	}
	//Render to screen
	SDL_RenderCopyEx( renderer, mTexture, clip, &renderQuad, angle, center, flip );
}

int LTexture::getWidth(){
	return mWidth;
}

int LTexture::getHeight(){
	return mHeight;
}

/*********TILE***********/
Tile::Tile( int x, int y, int tileType ){
    //Get the offsets
    mBox.x = x;
    mBox.y = y;

    //Set the collision box
    mBox.w = TILE_WIDTH;
    mBox.h = TILE_HEIGHT;

    //Get the tile type
    mType = tileType;
}

void Tile::render( SDL_Renderer* renderer, SDL_Rect& camera, double rotation ){
    //If the tile is on screen
    if( checkCollision( camera, mBox ) )
    {
        //Show the tile
        gTileTexture.render( renderer, mBox.x - camera.x, mBox.y - camera.y, &gTileClips[ mType ], rotation );
    }
}

int Tile::getType(){
    return mType;
}

SDL_Rect Tile::getBox(){
    return mBox;
}

/**********FROG***********/
Frog::Frog( int initX, int initY, double initRotation ){
    mBox.x = initX;
    mBox.y = initY;

    mBox.w = FROG_WIDTH;
    mBox.h = FROG_HEIGHT;

    mVelX = 0;
    mVelY = 0;

    isSpacePress = false;

    rotation = initRotation;
    /*mBox
    *   ####A####
    *   #   |   #
    *   #   O   #
    *   #       #
    *   #########
    *consider mBox has another Oxy system with O(0,0) and A(0, FROG_HEIGHT/2)
    */
    float initVtX = 0;//Ax
    float initVtY = FROG_HEIGHT/2; //Ay

    vtToGetDirection.setXandY( initVtX, initVtY );

}

void Frog::handleEvent( SDL_Event& e, double currentRotation, int frame ){
    if( e.type == SDL_KEYDOWN && e.key.repeat == 0){
        if (e.key.keysym.sym == SDLK_SPACE){
            isSpacePress = true;
            //getCurrentRotation
            setRotation( currentRotation );
            setVtGetDirectionViaRotation();
            //change mVelX and mVelY
            vtToGetDirection.normalize();
            mVelX = static_cast<int>(vtToGetDirection.getX()*FROG_VEL);
            mVelY = -static_cast<int>(vtToGetDirection.getY()*FROG_VEL);
        }
    }
    else if( e.type == SDL_KEYUP && e.key.repeat == 0 ){
            if(e.key.keysym.sym == SDLK_SPACE){

            }
    }

    if(!isSpacePress){
        mVelX = 0;
        mVelY = 0;
    }
}

void Frog::toMove( Tile* tiles[] ){
    //Move the frog left or right
    mBox.x += mVelX;

    //If the frog went too far to the left or right or touched a wall
    if( ( mBox.x < 0 ) || ( mBox.x + FROG_WIDTH > LEVEL_WIDTH ) || touchesWall( mBox, tiles ) )
    {
        //move back
        mBox.x -= mVelX;
    }

    //Move the frog up or down
    mBox.y += mVelY;

    //If the frog went too far up or down or touched a wall
    if( ( mBox.y < 0 ) || ( mBox.y + FROG_HEIGHT > LEVEL_HEIGHT ) || touchesWall( mBox, tiles ) )
    {
        //move back
        mBox.y -= mVelY;
    }
}

void Frog::render( SDL_Renderer* renderer, SDL_Rect& camera, SDL_Rect* currentClip, double currentRotation ){
    FrogTexture.render( renderer, mBox.x - camera.x, mBox.y - camera.y, currentClip, currentRotation );
}

void Frog::setCamera( SDL_Rect& camera ){
    //Center the camera over the frog
	camera.x = ( mBox.x + FROG_WIDTH / 2 ) - ui.SCREEN_WIDTH / 2;
	camera.y = ( mBox.y + FROG_HEIGHT / 2 ) - ui.SCREEN_HEIGHT / 2;

	//Keep the camera in bounds
	if( camera.x < 0 )
	{
		camera.x = 0;
	}
	if( camera.y < 0 )
	{
		camera.y = 0;
	}
	if( camera.x > LEVEL_WIDTH - camera.w )
	{
		camera.x = LEVEL_WIDTH - camera.w;
	}
	if( camera.y > LEVEL_HEIGHT - camera.h )
	{
		camera.y = LEVEL_HEIGHT - camera.h;
	}
}

void Frog::setBox( int& boxX, int& boxY ){
    mBox.x = boxX;
    mBox.y = boxY;
}

void Frog::setRotation(double _rotation){
    rotation = _rotation;
}

void Frog::setVtGetDirectionViaRotation(){
    float radians = rotation * M_PI / 180.0;
    float sn = sin(-radians); // -radians to calculate clockwise
    float cs = cos(-radians);
    float x = 0;
    float y = FROG_HEIGHT/2;

    /*mBox
    *   ####A####
    *   #   |   #
    *   #   O   #
    *   #       #
    *   #########
    */
    // calculate new vector when OA rotate radian clockwise
    float newX = x * cs - y * sn;
    float newY = x * sn + y * cs;

    vtToGetDirection.setXandY(newX, newY);
}

/*********VECTOR2D************/
Vector2D:: Vector2D(float _x, float _y):x(_x), y(_y){};

Vector2D::~Vector2D() {};

Vector2D Vector2D::operator+(const Vector2D& Other) const {
    return Vector2D {
        x + Other.x,
        y + Other.y
    };
}

Vector2D Vector2D::operator- (const Vector2D& Other) const {
    return Vector2D {
      x - Other.x,
      y - Other.y,
    };
  }

Vector2D Vector2D::operator* (float Multiplier) const {
    return Vector2D {
      x * Multiplier,
      y * Multiplier,
    };
  }

Vector2D Vector2D:: operator/ (float Divisor) const {
    return Vector2D {
        x / Divisor,
        y / Divisor,
    };
}

Vector2D Vector2D::operator+= (const Vector2D& Other) {
    x += Other.x;
    y += Other.y;
    return *this;
  }

Vector2D Vector2D:: operator-= (const Vector2D& Other) {
    x -= Other.x;
    y -= Other.y;
    return *this;
}

Vector2D Vector2D::operator*= (float Multiplier) {
    x *= Multiplier;
    y *= Multiplier;
    return *this;
}

Vector2D Vector2D::operator- () const {
    return Vector2D {-x, -y};
}
bool Vector2D::operator==  (const Vector2D& other) const {
    return (x == other.x && y == other.y);
}

void Vector2D::normalize() {
        float len = length();
        if (len > 0) {
            x /= len;
            y /= len;
        }
    }

float Vector2D::length(){
    return sqrt(x*x + y*y);
}


/*********LEAF*************/
Leaf::Leaf( Tile* _tileLeaf, double _angle, int _rotateSpeedCoeffcient ){
    tileLeaf = _tileLeaf;

    mAngle = _angle;

    rotateSpeedCoeffcient = _rotateSpeedCoeffcient;

    isFrogOn = false;

    wasFrogOn = false;
}

Leaf::~Leaf(){
    delete tileLeaf;
    tileLeaf = nullptr;
}

void Leaf::setFrogCenter( Frog& _frog ){

    int XpoCenterLeaf = tileLeaf->getBox().x + tileLeaf->getBox().w/2;
    int YpoCenterLeaf = tileLeaf->getBox().y + tileLeaf->getBox().h/2;

    int XnewOfFrogBox = XpoCenterLeaf - _frog.FROG_WIDTH/2;
    int YnewOfFrogBox = YpoCenterLeaf - _frog.FROG_HEIGHT/2;

    _frog.setBox( XnewOfFrogBox, YnewOfFrogBox );
}

void Leaf::rotation(){

    mAngle += rotateSpeedCoeffcient * ROTATION_SPEED;
    if( mAngle == 360 ){
        mAngle = 0;
    }
}

/*********GAME*FUNTION********/
bool loadMedia( Tile* tiles[], SDL_Renderer* renderer ){
	//Loading success flag
	bool success = true;

	//Load frog texture
	if( !FrogTexture.loadFromFile( "assets/images/frogAnimation.png", renderer ) )
	{
		printf( "Failed to load dot texture!\n" );
		success = false;
	}
	else{
        for (int i = 0; i < JUMP_ANIMATION_FRAME; i++){
            FrogClips[ i ].x = i*50;
            FrogClips[ i ].y = 0;
            FrogClips[ i ].w = 50;
            FrogClips[ i ].h = 50;
        }
	}

	//Load tile texture
	if( !gTileTexture.loadFromFile( "assets/images/leaf0.png", renderer ) )
	{
		printf( "Failed to load tile set texture!\n" );
		success = false;
	}

	//Load tile map
	if( !setTiles( tiles ) )
	{
		printf( "Failed to load tile set!\n" );
		success = false;
	}

	//Load background texture
	if( !gBGTexture.loadFromFile( "assets/images/bg.png", renderer ) )
	{
		printf( "Failed to load background texture!\n" );
		success = false;
	}
	if( !sandTexture.loadFromFile( "assets/images/sand.png", renderer ) )
	{
		printf( "Failed to load bg2 texture!\n" );
		success = false;
	}
    //set color for prompt text
	SDL_Color textColor = { 0, 0, 0, 0xFF };
	//SDL_Color highlightColor = { 0xFF, 0, 0, 0xFF };
	//Open the font
	gFont = TTF_OpenFont( "assets/textHandle/font.ttf", 32 );
	if( gFont == NULL )
	{
		printf( "Failed to load game font! SDL_ttf Error: %s\n", TTF_GetError() );
		success = false;
	}
	else
	{
		//Render the prompt
		if( !gPromptTextTexture.loadFromRenderedText( "SCORE: ", textColor, renderer, gFont ) )
		{
			printf( "Failed to render prompt text!\n" );
			success = false;
		}
	}


	return success;
}

void close( Tile* tiles[] ){
	//Deallocate tiles
	for( int i = 0; i < TOTAL_TILES; ++i )
	{
		 if( tiles[ i ] != NULL )
		 {
			delete tiles[ i ];
			tiles[ i ] = NULL;
		 }
	}

	//Free global font
	TTF_CloseFont( gFont );
	gFont = NULL;

	//Free loaded images
	FrogTexture.free();
	gTileTexture.free();
	gBGTexture.free();
	sandTexture.free();

	gPromptTextTexture.free();
	gScore.free();


	quitSDL( ui.window, ui.render );
}

bool checkCollision( SDL_Rect a, SDL_Rect b ){
   if(SDL_HasIntersection(&a, &b)){
    return true;
   }
   return false;
}

bool setTiles( Tile* tiles[] ){
	//Success flag
	bool tilesLoaded = true;

    //The tile offsets
    int x = 0, y = 0;

    //Open the map
    std::ifstream map( "assets/tileMap/tile.map" );

    //If the map couldn't be loaded
    if( map.fail() )
    {
		printf( "Unable to load map file!\n" );
		tilesLoaded = false;
    }
	else
	{
		//Initialize the tiles
		for( int i = 0; i < TOTAL_TILES; ++i )
		{
			//Determines what kind of tile will be made
			int tileType = -1;

			//Read tile from map file
			map >> tileType;

			//If the was a problem in reading the map
			if( map.fail() )
			{
				//Stop loading map
				printf( "Error loading map: Unexpected end of file!\n" );
				tilesLoaded = false;
				break;
			}

			//If the number is a valid tile number
			if( ( tileType >= 0 ) && ( tileType <= TOTAL_TILE_SPRITES ) )
			{
				tiles[ i ] = new Tile( x, y, tileType );
			}
			//If we don't recognize the tile type
			else
			{
				//Stop loading map
				printf( "Error loading map: Invalid tile type at %d!\n", i );
				tilesLoaded = false;
				break;
			}

			//Move to next tile spot
			x += TILE_WIDTH;

			//If we've gone too far
			if( x >= LEVEL_WIDTH )
			{
				//Move back
				x = 0;

				//Move to the next row
				y += TILE_HEIGHT;
			}
		}

		//Clip the sprite sheet
		if( tilesLoaded )
		{
			gTileClips[ TILE_LEAF ].x = 0;
			gTileClips[ TILE_LEAF ].y = 0;
			gTileClips[ TILE_LEAF ].w = TILE_WIDTH;
			gTileClips[ TILE_LEAF ].h = TILE_HEIGHT;
		}
	}

    //Close the file
    map.close();

    //If the map was loaded fine
    return tilesLoaded;
}

bool touchesWall( SDL_Rect box, Tile* tiles[] ){
    //Go through the tiles
    for( int i = 0; i < TOTAL_TILES; ++i )
    {
        //If the tile is a wall type tile
        if( ( tiles[ i ]->getType() >= TILE_CENTER ) && ( tiles[ i ]->getType() <= TILE_TOPLEFT ) )
        {
            //If the collision box touches the wall tile
            if( checkCollision( box, tiles[ i ]->getBox() ) )
            {
                return true;
            }
        }
    }

    //If no wall tiles were touched
    return false;
}

void setListLeaf( Tile* tiles[], vector<Leaf*>& _listLeaf ){
    int cnt = 1;
    double addSpeed = 5.0;
    for( int i = 0; i < TOTAL_TILES; i++){
        if( tiles[ i ]->getType() == TILE_LEAF ){
            Leaf* tileLeaf = new Leaf( tiles[ i ], i*15.0 , static_cast<int>(2 + addSpeed));
            _listLeaf.push_back( tileLeaf );
            cnt++;
            if(addSpeed > 0){
                addSpeed -= 0.3;
            }
            else addSpeed = 0.0;
        }
    }
}

void closeListLeaf( vector<Leaf*>& _listLeaf ){
    for( auto x : _listLeaf ){
        delete x;
    }
    _listLeaf.clear();
}

Leaf* getLastpLeaf( vector<Leaf*>& _listLeaf ){
     if (_listLeaf.empty()) {
        return nullptr;
    }
    return _listLeaf.back();
}

bool checkIsFrogLive( const vector<Leaf*>& _listLeaf ){
    int vtSize = _listLeaf.size();
    for(int i = 0; i < vtSize; i++){
        if( _listLeaf[ i ]->getIsFrogOn() ){
            return true;
        }
    }
    return false;
}

/************GAME*************/

bool StartScreen(){
    bool isStart = false;

    LTexture mFrog;
    LTexture mLeaf;
    LTexture play;
    LTexture exit;
    LTexture river;
    LTexture nameGame;
    LTexture treeShadow;

    bool success = true;
    if( !mFrog.loadFromFile( "assets/images/frogBlur.png", ui.render ) )
	{
		printf( "Failed to load frog texture!\n" );
		success = false;
	}
    if( !mLeaf.loadFromFile( "assets/images/leafBlur.png", ui.render ) )
	{
		printf( "Failed to load leaf texture!\n" );
		success = false;
	}
    if( !play.loadFromFile( "assets/images/play.png", ui.render ) )
	{
		printf( "Failed to load play texture!\n" );
		success = false;
	}
    if( !exit.loadFromFile( "assets/images/exit.png", ui.render ) )
	{
		printf( "Failed to load play texture!\n" );
		success = false;
	}
    if( !nameGame.loadFromFile( "assets/images/FROOG.png", ui.render ) )
	{
		printf( "Failed to load play texture!\n" );
		success = false;
	}
    if( !river.loadFromFile( "assets/images/riverBlur.png", ui.render ) )
	{
		printf( "Failed to load play texture!\n" );
		success = false;
	}
	 if( !treeShadow.loadFromFile( "assets/images/treeBlur.png", ui.render ) )
	{
		printf( "Failed to load play texture!\n" );
		success = false;
	}
	if(success){
        const int nameGameX = ui.SCREEN_WIDTH/2 - nameGame.getWidth()/2;
        const int nameGameY = 150;
        const int playX = ui.SCREEN_WIDTH/2 - play.getWidth()/2;
        const int playY = nameGameY + nameGame.getHeight() - 25;
        const int exitX = ui.SCREEN_WIDTH/2 - exit.getWidth()/2 - 10;
        const int exitY = playY + play.getHeight() - 10;
        SDL_Event e;
        int mX = 0, mY = 0;
        bool quit = false;
        int scrollingOffset = 0;
        double mRotation = 0;
        while (!quit){
            while(SDL_PollEvent(&e) != 0){
                if( e.type == SDL_QUIT ){
                    quit = true;
                    }
                if( e.type == SDL_MOUSEBUTTONDOWN && e.key.repeat == 0 )
                {
                    SDL_GetMouseState(&mX, &mY);
                }
            }
            if(mX > playX && mX < playX + play.getWidth() && mY > playY && mY < exitY){
                isStart = true;
                break;
            }
            if(mX > exitX && mX < exitX + play.getWidth() && mY > exitY && mY < exitY + exit.getHeight()){
                isStart = false;
                break;
            }
            scrollingOffset--;
            if( scrollingOffset < -river.getHeight() ){
                scrollingOffset = 0;
            }
            SDL_SetRenderDrawColor( ui.render, 0xFF, 0xFF, 0xFF, 0xFF );
            ui.clearRender();

            river.render( ui.render, 0, scrollingOffset );
            river.render( ui.render, 0, scrollingOffset + river.getHeight() );
            treeShadow.render( ui.render, 0, scrollingOffset );

            mLeaf.render(ui.render, playX, 600 + scrollingOffset, nullptr, mRotation);
            mFrog.render(ui.render, playX + 100/2 - 50/2, 600+ scrollingOffset + 100/2 - 50/2, nullptr, mRotation);

            nameGame.render(ui.render, nameGameX, nameGameY);
            play.render(ui.render, playX, playY);
            exit.render(ui.render, exitX, exitY);

            ui.present();

            SDL_Delay(16);
            mRotation++;
            if(mRotation == 360){
                mRotation = 0;
            }
        }

	}

    mFrog.free();
    mLeaf.free();
    play.free();
    exit.free();
    river.free();
    nameGame.free();
    treeShadow.free();

   return isStart;
}

bool guideScreen(){
    bool success = true;
    bool isStart = false;
    LTexture guide;
    if( !guide.loadFromFile( "assets/images/guide.png", ui.render ) )
	{
		printf( "Failed to load guide texture!\n" );
		success = false;
	}
	if(success){
        SDL_Event e;
        bool quit = false;
        while (!quit){
            while(SDL_PollEvent(&e) != 0){
                if( e.type == SDL_QUIT ){
                    quit = true;
                }
                if( e.type == SDL_KEYDOWN && e.key.repeat == 0 ){
                    if( e.key.keysym.sym == SDLK_SPACE ){
                        isStart = true;
                    }
                }
            }

            if(isStart == true){
                quit = true;
            }
            SDL_SetRenderDrawColor( ui.render, 0xFF, 0xFF, 0xFF, 0xFF );
            ui.clearRender();

            guide.render(ui.render, 0, 0);


            ui.present();
        }

	}
	guide.free();
	return isStart;

}

void ScoreInit(){
   gScore.loadFromRenderedText( to_string(score), scoreColor, ui.render, gFont );
}

void listLeafInit(){
    setListLeaf( tileSet, listLeaf );
}

void FrogInit(){
    Leaf* bottomLeaf = getLastpLeaf( listLeaf );
    int xInitFrogBox = bottomLeaf->getBox().x + bottomLeaf->getBox().w /2 - 25;
    int yInitFrogBox = bottomLeaf->getBox().y + bottomLeaf->getBox().h /2 - 25;
    double angleInitFrog = bottomLeaf->getAngle();
    Frog _frog( xInitFrogBox, yInitFrogBox , angleInitFrog );
    bottomLeaf->setIsFrogOn(true);
    bottomLeaf->setWasFrogOn(true);
    frog = _frog;
    score = 0;
}

bool GameInit(){
    bool success = true;
    if( !loadMedia( tileSet, ui.render ) )
    {
        printf( "Failed to load media!\n" );
        success = false;
    }
    if(success){
        ScoreInit();
        listLeafInit();
        FrogInit();
    }
    return success;
}

void GameHandleEvent(bool& playAgain, bool& quit, SDL_Event& e, int& frame){
    while( SDL_PollEvent( &e ) != 0 )
    {
        //User requests quit
        if( e.type == SDL_QUIT ){
            quit = true;
            playAgain = false;
        }
        if( e.type == SDL_KEYDOWN && e.key.repeat == 0){
            if (e.key.keysym.sym == SDLK_SPACE){
                currentRotation = rotation;
                score++;
            }
        }
        frog.handleEvent( e, currentRotation, frame );
    }
}

void GameRender(int& frame, const int& listLeafSize){
 //Clear screen
        SDL_SetRenderDrawColor( ui.render, 0xFF, 0xFF, 0xFF, 0xFF );
        ui.clearRender();

        //Render background
        sandTexture.render(ui.render, 0, -scrollingOffsetSand);
        sandTexture.render( ui.render, 0,-(scrollingOffsetSand + sandTexture.getHeight()) );
        gBGTexture.setBlendMode(SDL_BLENDMODE_MOD);
        gBGTexture.render( ui.render, 0, -scrollingOffsetBG );
        gBGTexture.render( ui.render, 0, -(scrollingOffsetBG + gBGTexture.getHeight()) );

        //Render level
        for(int i = listLeafSize-1; i >= 0; --i){
            listLeaf[ i ]->rotation();
            double currentAngle = listLeaf[ i ]->getAngle();
            listLeaf[ i ]->getTile()->render( ui.render, camera, currentAngle );
        }
        //Render frog
        SDL_Rect* currentFrogClip = &FrogClips[ frame ];
        if (frog.getIsSpacePress()){
            frog.render( ui.render, camera, currentFrogClip, currentRotation );
        }
        else{
            frog.render( ui.render, camera, &FrogClips[ 0 ] , rotation );
        }

        gPromptTextTexture.render( ui.render, ( ui.SCREEN_WIDTH - gPromptTextTexture.getWidth() ) / 2, 0 );
        gScore.loadFromRenderedText( to_string(score), scoreColor, ui.render, gFont );
        gScore.render( ui.render, ( ui.SCREEN_WIDTH + gPromptTextTexture.getWidth() ) / 2 , 0 );
        //Update screen
        ui.present();

        SDL_Delay(16);
}

void GameLoop(bool& playAgain){
    int vtSize = listLeaf.size();
    //Main loop flag
    bool quit = false;
    //frogFrame
    int frame = 0;
    //Event handler
    SDL_Event e;
    //set camera
    frog.setCamera( camera );
    //set BG offset
    scrollingOffsetBG = 0;
    scrollingOffsetSand = 0;
    //status live of frog
    bool isFrogLive = true;

    while( !quit )
    {
        GameHandleEvent(playAgain, quit, e, frame);
        //Move the frame
        if (frog.getIsSpacePress()){
            frog.toMove( tileSet );
            frog.setCamera( camera );
            ++frame;
            //Scroll background
            --scrollingOffsetSand;
        }
        --scrollingOffsetBG;
        if( scrollingOffsetBG < -gBGTexture.getHeight() ){
            scrollingOffsetBG = 0;
        }
        if( scrollingOffsetSand < -sandTexture.getHeight() ){
            scrollingOffsetSand = 0;
        }
        if(frog.getIsSpacePress()){
            for(int i = 0; i < vtSize; i++){
                listLeaf[ i ]->setIsFrogOn( false );
            }
        }
        //check next leaf
        if (!frog.getIsSpacePress()){
            for(int i = 0; i < vtSize; i++){
                if(listLeaf[ i ]->getWasFrogOn() == false){
                    listLeaf[ i ]->setIsFrogOn(checkCollision(frog.getBox(), listLeaf[ i ]->getBox()));
                    if( listLeaf[ i ]->getIsFrogOn() == true){
                        listLeaf[ i ]->setWasFrogOn( true );
                        listLeaf[ i ]->setFrogCenter( frog );
                    }
                }
            }
            if(!checkIsFrogLive(listLeaf)){
                isFrogLive = false;
            }
        }
        //Render game
        GameRender(frame, vtSize);

        for(int i = 0; i < vtSize; i++){
            if( listLeaf[ i ]->getIsFrogOn() ){
                rotation = listLeaf[ i ]->getAngle();
                break;
            }
        }
        if(!frog.getIsSpacePress()){
            frame = 0;
        }
        if( frame >= JUMP_ANIMATION_FRAME ){

            frog.setIsSpacePress(false);

        }
        if(rotation == 360){
            rotation = 0;
        }
        if(!isFrogLive){
            playAgain = gameOverScreen(quit);
        }
        if(listLeaf[ 0 ]->getWasFrogOn()){
            playAgain = gameWinnerScreen(quit);
        }
    }
}

void GameClose(){
    //Free resources and close SDL
    close( tileSet );
    closeListLeaf( listLeaf );
}

void GameRun(){
     bool playAgain = false;
     if(StartScreen()){
        if(guideScreen()){
            if(GameInit()){
                do{
                    if(playAgain){
                        FrogInitPlayAgain();
                        playAgain = false;
                    }
                    GameLoop(playAgain);
                }while(playAgain);
            }
        }
    }
    GameClose();
}

bool gameOverScreen(bool& quit){
    bool isAgain = true;
    LTexture background;
    LTexture againButton;
    LTexture exitButton;
    LTexture highScore;
    SDL_Color GO_scoreColor = { 255, 255, 102, 0xFF };
    TTF_Font* GO_font = TTF_OpenFont( "assets/textHandle/font.ttf", 100 );
    bool success = true;
	if( GO_font == NULL )
	{
		printf( "Failed to load gameover font! SDL_ttf Error: %s\n", TTF_GetError() );
		success = false;
	}
	else{
          gScore.loadFromRenderedText( to_string(score), GO_scoreColor, ui.render, GO_font );
	}
    if( !background.loadFromFile( "assets/images/gameover.png", ui.render ) )
	{
		printf( "Failed to load gameover background texture!\n" );
		success = false;
	}
    if( !againButton.loadFromFile( "assets/images/gameoveragain.png", ui.render ) )
	{
		printf( "Failed to load again button texture!\n" );
		success = false;
	}
    if( !exitButton.loadFromFile( "assets/images/gameoverexit.png", ui.render ) )
	{
		printf( "Failed to load exitbutton texture!\n" );
		success = false;
	}
	if(success){
        const int againX = 456;
        const int againY = 416;
        const int exitX = 238;
        const int exitY = 415;
        SDL_Event e;
        int mX = 0, mY = 0;
        bool mQuit = false;
        while (!mQuit){
            while(SDL_PollEvent(&e) != 0){
                if( e.type == SDL_QUIT ){
                    mQuit = true;
                    quit = true;
                    isAgain = false;
                }
                if( e.type == SDL_MOUSEBUTTONDOWN && e.key.repeat == 0 )
                {
                    SDL_GetMouseState(&mX, &mY);
                }
            }
            if(mX > againX && mX < againX + againButton.getWidth() && mY > againY && mY < againY + againButton.getHeight()){
                isAgain = true;
                quit = true;
                break;
            }
            if(mX > exitX && mX < exitX + exitButton.getWidth() && mY > exitY && mY < exitY + exitButton.getHeight()){
                quit = true;
                isAgain = false;
                break;
            }

            SDL_SetRenderDrawColor( ui.render, 0xFF, 0xFF, 0xFF, 0xFF );
            ui.clearRender();

            background.render(ui.render, 0, 0);
            againButton.render(ui.render, againX, againY);
            exitButton.render(ui.render, exitX, exitY);
            gScore.render(ui.render, 478, 275);

            ui.present();

            SDL_Delay(16);
            }
        }
    background.free();
    againButton.free();
    exitButton.free();
    highScore.free();
    TTF_CloseFont( GO_font );
	GO_font = NULL;
    return isAgain;
}

bool gameWinnerScreen(bool& quit){
    bool isAgain = true;
    LTexture background;
    LTexture againButton;
    LTexture exitButton;
    LTexture highScore;
    bool success = true;
    if( !background.loadFromFile( "assets/images/gamewin.png", ui.render ) )
	{
		printf( "Failed to load gamewin background texture!\n" );
		success = false;
	}
    if( !againButton.loadFromFile( "assets/images/gameoveragain.png", ui.render ) )
	{
		printf( "Failed to load again button texture!\n" );
		success = false;
	}
    if( !exitButton.loadFromFile( "assets/images/gameoverexit.png", ui.render ) )
	{
		printf( "Failed to load exit button texture!\n" );
		success = false;
	}
	if(success){
        const int againX = 456;
        const int againY = 416;
        const int exitX = 238;
        const int exitY = 415;
        SDL_Event e;
        int mX = 0, mY = 0;
        bool mQuit = false;
        while (!mQuit){
            while(SDL_PollEvent(&e) != 0){
                if( e.type == SDL_QUIT ){
                    mQuit = true;
                    quit = true;
                    isAgain = false;
                }
                if( e.type == SDL_MOUSEBUTTONDOWN && e.key.repeat == 0 )
                {
                    SDL_GetMouseState(&mX, &mY);
                }
            }
            if(mX > againX && mX < againX + againButton.getWidth() && mY > againY && mY < againY + againButton.getHeight()){
                isAgain = true;
                quit = true;
                break;
            }
            if(mX > exitX && mX < exitX + exitButton.getWidth() && mY > exitY && mY < exitY + exitButton.getHeight()){
                quit = true;
                isAgain = false;
                break;
            }

            SDL_SetRenderDrawColor( ui.render, 0xFF, 0xFF, 0xFF, 0xFF );
            ui.clearRender();

            background.render(ui.render, 0, 0);
            againButton.render(ui.render, againX, againY);
            exitButton.render(ui.render, exitX, exitY);

            ui.present();

            SDL_Delay(16);
            }
        }
    background.free();
    againButton.free();
    exitButton.free();
    return isAgain;

}

void FrogInitPlayAgain(){
    for (int i = 0, vtSize = listLeaf.size(); i < vtSize; i++){
        listLeaf[i]->setWasFrogOn(false);
        listLeaf[i]->setIsFrogOn(false);
    }
    Leaf* bottomLeaf = getLastpLeaf( listLeaf );
    int xInitFrogBox = bottomLeaf->getBox().x + bottomLeaf->getBox().w /2 - 25;
    int yInitFrogBox = bottomLeaf->getBox().y + bottomLeaf->getBox().h /2 - 25;
    double angleInitFrog = bottomLeaf->getAngle();
    frog.setBox(xInitFrogBox, yInitFrogBox);
    frog.setRotation(angleInitFrog);
    bottomLeaf->setIsFrogOn(true);
    bottomLeaf->setWasFrogOn(true);
    score = 0;
}
