// To run
// gcc -Isrc/Include -Lsrc/lib -o main main.c -lmingw32 -lSDL2main -lSDL2


#include <stdio.h>
#include <math.h>
#include <SDL2/SDL.h>

#define WIDTH 1800
#define HEIGHT 900

#define COLOR_WHITE 0xFFFFFFFF
#define COLOR_BLACK 0x00000000
#define COLOR_YELLOW 0xFFFFD700
#define COLOR_GREEN 0xFF00FF00

#define AU 1.496 * pow(10, 11)
#define G 6.674 * pow(10, -11)

#define MASS_SUN 1.989 * pow(10, 30)
#define MASS_EARTH 5.972 * pow(10, 24)
#define MASS_MARS 6.39 * pow(10, 23)
#define MASS_MERCURY 3.3011 * pow(10, 23)
#define MASS_VENUS 4.8675 * pow(10, 24)
#define MASS_JUPITER 1.8982 * pow(10, 27)
#define MASS_SATURN 5.6834 * pow(10, 26)
#define MASS_URANUS 8.6810 * pow(10, 25)
#define MASS_NEPTUNE 1.0241 * pow(10, 26)

#define INITIAL_VELOCITY 29780.8 
#define INITIAL_VELOCITY_MERCURY 47360.0
#define INITIAL_VELOCITY_VENUS 35020.0
#define INITIAL_VELOCITY_MARS 24077.0
#define INITIAL_VELOCITY_JUPITER 13070.0
#define INITIAL_VELOCITY_SATURN 9690.0
#define INITIAL_VELOCITY_URANUS 6810.0
#define INITIAL_VELOCITY_NEPTUNE 5430.0

#define DT 800000 // CHANGE FOR FASTER, RIGHT NOW 1SEC REAL = 100000 SECONDS IN SIM
#define SPACE 13 // SPACE BETWEEN PLANETS
#define SCALE SPACE / (AU) // ESCALA 10 pixeis = AU

#define SECONDS_YEAR_MERCURY 7.600*pow(10,6)
#define SECONDS_YEAR_VENUS 1.941*pow(10, 7)
#define SECONDS_YEAR_EARTH 3.156*pow(10, 7)
#define SECONDS_YEAR_MARS 5.935*pow(10, 7)
#define SECONDS_YEAR_JUPITER 3.743*pow(10, 8)
#define SECONDS_YEAR_SATURN 9.294*pow(10, 8)
#define SECONDS_YEAR_URANUS 2.651*pow(10, 9)
#define SECONDS_YEAR_NEPTUNE 5.200*pow(10, 9)

#define NUMBER_OF_POINTS_MERCURY SECONDS_YEAR_MERCURY/ DT
#define NUMBER_OF_POINTS_VENUS SECONDS_YEAR_VENUS / DT
#define NUMBER_OF_POINTS (SECONDS_YEAR_EARTH) / DT
#define NUMBER_OF_POINTS_MARS SECONDS_YEAR_MARS / DT
#define NUMBER_OF_POINTS_JUPITER SECONDS_YEAR_JUPITER / DT
#define NUMBER_OF_POINTS_SATURN SECONDS_YEAR_SATURN / DT
#define NUMBER_OF_POINTS_URANUS SECONDS_YEAR_URANUS / DT
#define NUMBER_OF_POINTS_NEPTUNE SECONDS_YEAR_NEPTUNE / DT

#define NUMBER_OF_PLANETS 9 // Fixed to 3

struct Point {
    int x, y;
};

struct Circle {
    double x;
    double y;
    double x_m; // pos x of planet in m
    double y_m;
    double r;   // radius of planet
    double mass; // mass of planet
    double x_m_prev; // Velocity x of planet
    double y_m_prev; // Velocity y of planet
};

void FillCircle(SDL_Surface* surface, struct Circle circle, Uint32 color) {
    double radius_squared = pow(circle.r, 2);
    for (double x = circle.x - circle.r; x <= circle.x + circle.r; x++) {
        for (double y = circle.y - circle.r; y <= circle.y + circle.r; y++) {
            double distance_squared = pow(x - circle.x, 2) + pow(y - circle.y, 2);
            if (distance_squared < radius_squared) {
                SDL_Rect pixel = (SDL_Rect){x, y, 1, 1};
                SDL_FillRect(surface, &pixel, color);
            }
        }
    }
}

void UpdateValues(int index_planet, int index_points, struct Circle* planet, struct Circle* PLANETS, struct Point* points) {
    double a_x = 0;
    double a_y = 0;

    for (int i = 0; i < NUMBER_OF_PLANETS; i++) {
        if (i == index_planet) continue;
        struct Circle* other_planet = &PLANETS[i];
        double distance = sqrt(pow(planet->x_m - other_planet->x_m, 2) + pow(planet->y_m - other_planet->y_m, 2));

        a_x -= (G * other_planet->mass * planet->x_m) / pow(distance, 3);
        a_y -= (G * other_planet->mass * planet->y_m) / pow(distance, 3);
    }



    double new_x = 2*planet->x_m - planet->x_m_prev + a_x*pow(DT, 2);
    double new_y = 2*planet->y_m - planet->y_m_prev + a_y*pow(DT, 2);

    planet->x_m_prev = planet->x_m;
    planet->y_m_prev = planet->y_m;

    planet->x_m = new_x;
    planet->y_m = new_y;


    planet->x = WIDTH / 2 + planet->x_m * SCALE;
    planet->y = HEIGHT / 2 - planet->y_m * SCALE;

    points[index_points].x = planet->x;
    points[index_points].y = planet->y;
}

int main(int argc, char* argv[]) {
    int traj = 0;
    int run = 0;

    int i;
    int i_earth=0;
    int i_mercury=0;
    int i_venus = 0;
    int i_mars = 0;
    int i_jupiter =0;
    int i_saturn =0;
    int i_uranus =0;
    int i_neptune=0;

    int index_mercury=0;
    int index_venus=0;
    int index = 0;
    int index_mars = 0;
    int index_jupiter=0;
    int index_saturn=0;
    int index_uranus=0;
    int index_neptune=0;

    int simulation_running = 1;

    SDL_Init(SDL_INIT_VIDEO); // Initialize video mode
    SDL_Window* window = SDL_CreateWindow("Planets", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, 0); // Create window
    SDL_Surface* surface = SDL_GetWindowSurface(window);

    struct Circle SUN = {WIDTH / 2, HEIGHT / 2, 0, 0, 10, MASS_SUN, 0, 0};
    struct Circle MERCURY = {(WIDTH / 2)+SPACE*1, HEIGHT / 2, 0.387*AU, 0, 5, MASS_MERCURY, 0.387*AU, INITIAL_VELOCITY_MERCURY*DT};
    struct Circle VENUS = {(WIDTH / 2)+SPACE*2, HEIGHT / 2, 0.723*AU, 0, 5, MASS_VENUS, 0.723*AU, INITIAL_VELOCITY_VENUS*DT};
    struct Circle EARTH = {(WIDTH / 2)+SPACE*3, HEIGHT / 2, 1 * AU, 0, 5, MASS_EARTH, 1 * AU, INITIAL_VELOCITY*DT};
    struct Circle MARS = {(WIDTH / 2)+SPACE*4, HEIGHT / 2, 1.524 * AU, 0, 5, MASS_MARS, 1.524 * AU, INITIAL_VELOCITY_MARS*DT};
    struct Circle JUPITER = {(WIDTH / 2)+SPACE*5, HEIGHT / 2, 5.203*AU, 0, 5, MASS_JUPITER, 5.203*AU, INITIAL_VELOCITY_JUPITER*DT};
    struct Circle SATURN = {(WIDTH / 2)+SPACE*6, HEIGHT / 2, 9.537*AU, 0, 5, MASS_SATURN, 9.537*AU, INITIAL_VELOCITY_SATURN*DT};
    struct Circle URANUS = {(WIDTH / 2)+SPACE*7, HEIGHT / 2, 19.191*AU, 0, 5, MASS_URANUS, 19.191*AU, INITIAL_VELOCITY_URANUS*DT};
    struct Circle NEPTUNE = {(WIDTH / 2)+SPACE*8, HEIGHT / 2, 30.069*AU, 0, 5, MASS_NEPTUNE, 30.069*AU, INITIAL_VELOCITY_NEPTUNE*DT};
    struct Circle* PLANETS = (struct Circle*)malloc(NUMBER_OF_PLANETS * sizeof(struct Circle));
    PLANETS[0] = SUN;
    PLANETS[1] = MERCURY;
    PLANETS[2] = VENUS;
    PLANETS[3]= EARTH;
    PLANETS[4]= MARS;
    PLANETS[5]= JUPITER;
    PLANETS[6]= SATURN;
    PLANETS[7]= URANUS;
    PLANETS[8]= NEPTUNE;

    struct Point* points_mercury = (struct Point*)malloc(NUMBER_OF_POINTS_MERCURY * sizeof(struct Point));
    struct Point* points_venus = (struct Point*)malloc(NUMBER_OF_POINTS_VENUS * sizeof(struct Point));
    struct Point* points_earth = (struct Point*)malloc(NUMBER_OF_POINTS * sizeof(struct Point));
    struct Point* points_mars = (struct Point*)malloc(NUMBER_OF_POINTS_MARS * sizeof(struct Point));
    struct Point* points_jupiter = (struct Point*)malloc(NUMBER_OF_POINTS_JUPITER * sizeof(struct Point));
    struct Point* points_saturn = (struct Point*)malloc(NUMBER_OF_POINTS_SATURN * sizeof(struct Point));
    struct Point* points_uranus = (struct Point*)malloc(NUMBER_OF_POINTS_URANUS * sizeof(struct Point));
    struct Point* points_neptune = (struct Point*)malloc(NUMBER_OF_POINTS_NEPTUNE * sizeof(struct Point));

    SDL_Event event;

    while (simulation_running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                simulation_running = 0;
            }
            if (event.type == SDL_MOUSEBUTTONDOWN) { // A mouse button was pressed
                if (event.button.button == SDL_BUTTON_LEFT) { // Check which button
                    int x_mouse = event.button.x;
                    int y_mouse = event.button.y;

                    if (x_mouse >= 1750 && x_mouse <= 1785 && y_mouse >= 850 && y_mouse <= 885) traj = 0;
                    if (x_mouse >= 15 && x_mouse <= 50 && y_mouse >= 850 && y_mouse <= 885) run = 0;
                }
                if (event.button.button == SDL_BUTTON_RIGHT) { // Check which button
                    int x_mouse = event.button.x;
                    int y_mouse = event.button.y;

                    if (x_mouse >= 1750 && x_mouse <= 1785 && y_mouse >= 850 && y_mouse <= 885) traj = 1;
                    if (x_mouse >= 15 && x_mouse <= 50 && y_mouse >= 850 && y_mouse <= 885) run = 1;
                }
            }
        }

        // Clear screen
        SDL_FillRect(surface, NULL, COLOR_BLACK);

        FillCircle(surface, SUN, COLOR_YELLOW);
        FillCircle(surface, MERCURY, SDL_MapRGB(surface->format, 128, 128, 128)); // Mercury (Gray)
        FillCircle(surface, VENUS, SDL_MapRGB(surface->format, 238, 232, 170));   // Venus (Pale Yellow)
        FillCircle(surface, EARTH, SDL_MapRGB(surface->format, 0, 0, 255));       // Earth (Blue)
        FillCircle(surface, MARS, SDL_MapRGB(surface->format, 193, 68, 14));      // Mars (Reddish-Brown)
        FillCircle(surface, JUPITER, SDL_MapRGB(surface->format, 255, 165, 0));   // Jupiter (Orange)
        FillCircle(surface, SATURN, SDL_MapRGB(surface->format, 218, 165, 32));   // Saturn (Pale Gold)
        FillCircle(surface, URANUS, SDL_MapRGB(surface->format, 173, 216, 230));  // Uranus (Pale Blue)
        FillCircle(surface, NEPTUNE, SDL_MapRGB(surface->format, 0, 0, 139));  

        if (traj == 1) 
        
        {
            for (int i = 0; i < NUMBER_OF_POINTS_MERCURY; i++) 
            {
                SDL_Rect pixel_mercury = (SDL_Rect){points_mercury[i].x, points_mercury[i].y, 1, 1};
                SDL_FillRect(surface, &pixel_mercury, COLOR_WHITE);
            }

            for (int i = 0; i < NUMBER_OF_POINTS_VENUS; i++) 
            {
                SDL_Rect pixel_venus = (SDL_Rect){points_venus[i].x, points_venus[i].y, 1, 1};
                SDL_FillRect(surface, &pixel_venus, COLOR_WHITE);
            }

            for (int i = 0; i < NUMBER_OF_POINTS_MARS; i++) 
            {
                SDL_Rect pixel_mars = (SDL_Rect){points_mars[i].x, points_mars[i].y, 1, 1};
                SDL_FillRect(surface, &pixel_mars, COLOR_WHITE);
            }

            for (int i = 0; i < NUMBER_OF_POINTS; i++) 
            {
                SDL_Rect pixel_earth = (SDL_Rect){points_earth[i].x, points_earth[i].y, 1, 1};
                SDL_FillRect(surface, &pixel_earth, COLOR_WHITE);
            }
            for (int i = 0; i < NUMBER_OF_POINTS_JUPITER; i++) 
            {
                SDL_Rect pixel_jupiter = (SDL_Rect){points_jupiter[i].x, points_jupiter[i].y, 1, 1};
                SDL_FillRect(surface, &pixel_jupiter, COLOR_WHITE);
            }

            for (int i = 0; i < NUMBER_OF_POINTS_SATURN; i++) 
            {
                SDL_Rect pixel_saturn = (SDL_Rect){points_saturn[i].x, points_saturn[i].y, 1, 1};
                SDL_FillRect(surface, &pixel_saturn, COLOR_WHITE);
            }

            for (int i = 0; i < NUMBER_OF_POINTS_URANUS; i++) 
            {
                    SDL_Rect pixel_uranus = (SDL_Rect){points_uranus[i].x, points_uranus[i].y, 1, 1};
                    SDL_FillRect(surface, &pixel_uranus, COLOR_WHITE);
            }

            for (int i = 0; i < NUMBER_OF_POINTS_NEPTUNE; i++) 
            {
                SDL_Rect pixel_neptune = (SDL_Rect){points_neptune[i].x, points_neptune[i].y, 1, 1};
                SDL_FillRect(surface, &pixel_neptune, COLOR_WHITE);
            }

            index++;
            index_mars++;
            index_mercury++;
            index_venus++;
            index_jupiter++;
            index_saturn++;
            index_uranus++;
            index_neptune++;
        }
        

        SDL_Rect BUTTON_TRAJ = (SDL_Rect){1750, 850, 35, 35};
        SDL_FillRect(surface, &BUTTON_TRAJ, COLOR_WHITE);

        SDL_Rect BUTTON_RUN = (SDL_Rect){15, 850, 35, 35};
        SDL_FillRect(surface, &BUTTON_RUN, COLOR_WHITE);

        if (run == 1) {
            UpdateValues(1, index_mercury, &MERCURY, PLANETS, points_mercury);
            UpdateValues(2, index_venus, &VENUS, PLANETS, points_venus);
            UpdateValues(3, index, &EARTH, PLANETS, points_earth);
            UpdateValues(4, index_mars, &MARS, PLANETS, points_mars);
            UpdateValues(5, index_jupiter, &JUPITER, PLANETS, points_jupiter);
            UpdateValues(6, index_saturn, &SATURN, PLANETS, points_saturn);
            UpdateValues(7, index_uranus, &URANUS, PLANETS, points_uranus);
            UpdateValues(8, index_neptune, &NEPTUNE, PLANETS, points_neptune);
        }
        

        if (index == round(NUMBER_OF_POINTS))
        {
            index = 0; 
            i_earth++;
        } 
       
           
        if (index_mars == round(NUMBER_OF_POINTS_MARS)) 
        {
            index_mars = 0;
            
            if(i_mars == 0)
            {
                printf("MARS TOOK %d DAYS = %d YEARS\n", (i_earth*364)+index, i_earth ); 
                i_mars++;
            }
        }
        if (index_mercury == round(NUMBER_OF_POINTS_MERCURY)) 
        {
            index_mercury = 0;
            
            if(i_mercury == 0)
            {
                printf("MERCURY TOOK %d DAYS = %d YEARS\n", (i_earth*364)+index, i_earth ); 
                i_mercury++;
            }
        }
        if (index_venus == round(NUMBER_OF_POINTS_VENUS)) 
        {
            index_venus = 0;
            
            if(i_venus == 0)
            {
                printf("VENUS TOOK %d DAYS = %d YEARS\n", (i_earth*364)+index, i_earth ); 
                i_venus++;
            }
        }
        if (index_jupiter == round(NUMBER_OF_POINTS_JUPITER)) 
        {
            index_jupiter = 0;
            
            if(i_jupiter == 0)
            {
                printf("JUPITER TOOK %d DAYS = %d YEARS\n", (i_earth*364)+index, i_earth ); 
                i_jupiter++;
            }
        }
        if (index_saturn == round(NUMBER_OF_POINTS_SATURN)) 
        {
            index_saturn = 0;
            
            if(i_saturn == 0)
            {
                printf("SATURN TOOK %d DAYS = %d YEARS\n", (i_earth*364)+index, i_earth ); 
                i_saturn++;
            }
        }
        if (index_uranus == round(NUMBER_OF_POINTS_URANUS)) 
        {
            index_uranus = 0;
            
            if(i_uranus == 0)
            {
                printf("URANUS TOOK %d DAYS = %d YEARS\n", (i_earth*364)+index, i_earth ); 
                i_uranus++;
            }
        }
        if (index_neptune == round(NUMBER_OF_POINTS_NEPTUNE)) 
        {
            index_neptune = 0;
            
            if(i_neptune == 0)
            {
                printf("NEPTUN TOOK %d DAYS = %d YEARS\n", (i_earth*364)+index, i_earth ); 
                i_neptune++;
            }
        }


        SDL_UpdateWindowSurface(window);
        SDL_Delay(10); // 100fps
        
       
    }

    // Free allocated memory
    free(points_earth);
    free(points_mars);
    free(points_mercury);
    free(points_venus);
    free(points_jupiter);
    free(points_saturn);
    free(points_uranus);
    free(points_neptune);
    free(PLANETS);

    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}