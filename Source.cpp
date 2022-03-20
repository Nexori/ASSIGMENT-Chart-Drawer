//Remark about the program
/*
This program is written on top of the project which evaluated a single function value, which has been sent to me.
It is due to some compilation error which I could not solve, while I was writing it separate project, although everything 
was copied from here (and it was working here...). Moreover, I did not manage to fix the problem with tangent functions, although alternative form of tangent
function tg = sinx/cosx does work, however the infinity value is not displayed correctly.
*/

//Remark about the SFML Library
/*
This program uses SFML library (https://www.sfml-dev.org/), which requires a proper setup available here: (https://www.sfml-dev.org/tutorials/2.5/start-vc.php)
However, in order to make life easier, I used MAKROs (.props) files available online, which speeds up whole setup process, and reduces a possibility of making a mistake.
This file is included in main .zip file, named SFML.prop, which has to be manually included to project using property manager. Keep in mind
that it is required to edit the nr.5 entry ( <SFMLDirectory>c:\Users\Krzysiek\Desktop\Visual Studio Repos\LIBS\SFML\SFML-2.5.1\</SFMLDirectory> )
to a folder where SFML library has been unpacked. It can be done using notepad or any form of text editor capable of opening it.
If everything is done properly, you should be able to compile the program. One more thing. While downloading the library, please download the 32bit version of SFML 
for this project, since this project is compiled in 32bit architecture. Otherwise you will get machine type conflict error and you will not be able to compile this code.
*/
#define _CRT_SECURE_NO_WARNINGS 1
#include <iostream>
#include <vector>
#include <cstdlib>
#include <string>
#include "Function.h"
#include "evaluate.h"
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Main.hpp>

using namespace std;

void findAndReplaceAll(std::string & input, std::string toSearch, std::string replaceStr)
{
	// Get the first occurrence
	size_t pos = input.find(toSearch);
	// Repeat till end is reached
	while (pos != std::string::npos)
	{
		// Replace this occurrence of Sub String
		input.replace(pos, toSearch.size(), replaceStr);
		// Get the next occurrence from the current position
		pos = input.find(toSearch, pos + replaceStr.size());
	}
}

// This function is simillar to the previous one (findAndReplaceAll)
void replacedoublenegative (std::string &input){
	string toSearch("--");
	string toReplace("+");
	size_t pos = input.find(toSearch);
	while (pos != std::string::npos) {
		input.replace(pos, toSearch.size(), toReplace);
		pos = input.find(toSearch, pos + toReplace.size());
	}
}

int main()
{
	//These variables were required by phaser function
	int a, ec;  
	char expr[1024];

	//These values were required for SFML part of the code
	int Xshift = WINX / 2;
	int shrink = 50; //value of 50 pixels will mean value 1 on X-plane  
	int size = (WINX);
	printf("Enter how many pixels on the screen will be an equivalent to value 1 on a X-plane(recommended 50):\n");
	scanf("%d", &shrink);

	//This part specifies if you want to print the values used in calculation (useful for debugging)
	int debug = 0;
	printf("Would you like the debug values to be displayed in the console? (y/n)\nNote: It may take a while to print all values, what will result in a significant delay.\n");
	scanf("%s", &debug);

	int flag = 0;
	// flag values: 0 - false ; 1 - true ;
	// y - 121, n - 110, Y - 89, N - 78 
	if (debug == 121 || debug == 89 || debug == 1)
		flag = 1;
	if (debug == 110 || debug == 78 || debug == 0)
		flag = 0;


	while (1) {
		//Create a dynamic array of class Function, named ArrayOfVariables
		vector<Function> ArrayOfVariables(size);

		printf("Enter an expression:\n");
		scanf("%s", &expr);
		expr[1023] = 0;
		trimspaces(expr);
		
		// min max values of Ydisp, which are going to be scaled up/down //
		double Ymax = 0;
		double Ymin = WINY;
		printf("Calculating function values...\n\n");
		// Replace X's in string with values and putting them into array //
			for (double j2 = ((-WINX / 2)); j2 < (WINX / 2); j2++)
			{
				int q;
				double p = j2 / shrink; // 1 on xplane becomes wide 50 [px]

				//Q is j2 shifted by negative values of X axis
				//Otherwise, the reference index would be negative
				q = (j2 + ((Xshift)));

				//converts Xvalue to string
				std::string xval = std::to_string(p); 
				std::string equation(expr);
				findAndReplaceAll(equation, "x", xval);
				findAndReplaceAll(equation, "X", xval);
				replacedoublenegative(equation);
				if ((j2) > (WINX / 2)) 
					break;

				//Here the string is converted into C-string so it can be used by pharser
				char TempConversion[1024];
				strcpy(TempConversion, equation.c_str());
				ec = evaluate(TempConversion, &ArrayOfVariables[q].y, &a);
				
				// The value from phaser is written into the class. Here also, the Xdisp and Ydisp values are calculated.
				ArrayOfVariables[q].x = (j2);
				ArrayOfVariables[q].y = ArrayOfVariables[q].y;
				ArrayOfVariables[q].Xdisp = (ArrayOfVariables[q].x + Xshift);
				ArrayOfVariables[q].Ydisp = ((WINY / 2) - ArrayOfVariables[q].y);

				if (expr[0] == 0)
					break;
				
				// Noting minimum and maximum values (will be used later on)
				if ((ArrayOfVariables[q].Ydisp > Ymax)) 
					Ymax = ArrayOfVariables[q].Ydisp;
				if (ArrayOfVariables[q].Ydisp == INFINITY) 
					Ymax = 0;
				if (ArrayOfVariables[q].Ydisp == (-INFINITY))
					Ymin = WINY;

				if ((ArrayOfVariables[q].Ydisp < Ymin))
					Ymin = ArrayOfVariables[q].Ydisp;
				
				// part of debug values
				if (flag == 1)
				{
					if (ec == E_OK)
						printf("%0.f \t X = %5.1f \t Y = %5.1f ||| Xdisp=%5.1f \t Ydisp=%5.1f \n", j2, ArrayOfVariables[q].x, ArrayOfVariables[q].y, ArrayOfVariables[q].Xdisp, ArrayOfVariables[q].Ydisp);
				else
					if (ec == E_EMPTY)
						printf("Empty expression\n");
					else
					{
						printf("ERROR: %s - %s", ErrMsgs[ERROR - 1], ERTOK);
						printf("\n%s", ERANC);
						printf("\n%*s^\n", ERPOS, "");
					}
				}


			}
			double deltaY = abs(Ymax-Ymin);
			//Scaling the points to be displayed//
			printf("Rescaling Y values...\n\n");
			if (deltaY <= WINY) //checks if difference between Ymin and Ymax is in reasonable range. If it was always true, it would 
			{

				double deltaplus = (WINY/2)/(Ymin-(WINY / 2));			//checks how much value changed (for Ydisp above X-axis)
				double deltaminus = (WINY / 2)/(Ymax -(WINY / 2));		//checks how much value chandeg (for Ydisp below X-axis)
				if (deltaminus >= WINY) 
					deltaminus = -deltaplus;
				if (deltaminus <= 0) 
					deltaminus = -deltaplus;
				if (deltaplus >= WINY)
					deltaplus = -deltaminus;
				if (deltaplus <= 0) 
					deltaplus = -deltaminus;

					for (int j2 = 0; j2 < WINX; j2++)
					{
						// part of debug values
					 if (flag == 1) printf("======\nNR. %d || Ydisp = %f  ", j2, ArrayOfVariables[j2].Ydisp);
						//amplifies the value upwards 
						if (ArrayOfVariables[j2].Ydisp > (WINY / 2)) {
							double delta = (((WINY / 2) - ArrayOfVariables[j2].Ydisp) * 9) / 10;			//note, default rescale is 90% of whole window 
							ArrayOfVariables[j2].Ydisp2 = ArrayOfVariables[j2].Ydisp + delta * deltaplus;	//if you wish to change it, simply change 
						}																					//the value of '9' and '10'
						//copies value, does not change anything											//according to formula = (9/10)*100%
						if (ArrayOfVariables[j2].Ydisp == (WINY / 2)) {										
							ArrayOfVariables[j2].Ydisp2 = ArrayOfVariables[j2].Ydisp;
						}
						//amplifies the value downwards
						if (ArrayOfVariables[j2].Ydisp < (WINY / 2)) {
							double delta = ((ArrayOfVariables[j2].Ydisp - (WINY / 2)) * 9) / 10;
							ArrayOfVariables[j2].Ydisp2 = ArrayOfVariables[j2].Ydisp + delta * deltaminus;
						}
						// part of debug values
						if (flag == 1) printf("Ydisp' = %f\n", ArrayOfVariables[j2].Ydisp2);
					}
					// part of debug values
				if (flag == 1) printf("\n ===================== \nRESCALED VALUES:\nScale dp %0.4f\nScale dm %0.4f\n", deltaplus, deltaminus);
			}

			else //if scaling was not performed, copy the values Ydisp as rescaled Ydisp2 (ony Ydisp2 is used in dispalying function)
				for (int i = 0; i < WINX; i++)
			{
				ArrayOfVariables[i].Ydisp2 = ArrayOfVariables[i].Ydisp;
			}
			
			// Part of debug information
			if (flag == 1)
			{
				printf("\n\n Ydisp minimum = %0.2f || Ydisp maximum = %0.2f  ||  deltaY = %0.2f", Ymin, Ymax, deltaY);
				printf("\nAlso note that SFML display works as follows:\n (Xdisp,Ydisp)\n    (0,0) ===========================================> (800,0)(window width)\n\t||\n\t||\n\t||\n\t||\n\t||\n\t||\n\t||\n\t||\n\tV\n\t(0,600)\n\t(screen height)\n");
				printf("\n========NEW FUNCTION=========\n(to specify a new funciton, please close the window on which function is displayed)\n\n\n\n\n\n");
			}

			// ---------- THE SFML PART OF THE PROGRAM -----------------//
			//create the main window on which the function will be displayed
			sf::RenderWindow window(sf::VideoMode(WINX, WINY), "Chart creator, version: 0.2 Alpha");
			
			// create a function line
			sf::VertexArray function(sf::LineStrip, size);
			for (int i = 0; i < size; i++) {
				function[i].position = sf::Vector2f(ArrayOfVariables[i].Xdisp, ArrayOfVariables[i].Ydisp2);
				function[i].color = sf::Color::Red; // feel free to change the color: eg sf::Color::Blue/White ...
			}
			//create markers on a X-axis
			sf::VertexArray xmark(sf::Lines, 2*(WINX / shrink));
			for (int i = 0, j = 0,k = 0; j < (WINX / shrink)*2; i++, j=j+2,k++) 
			{
				xmark[j].position	=	sf::Vector2f(0 + (j*shrink)/2, (WINY / 2) - 5);
				xmark[j+1].position	=	sf::Vector2f(0 + (j*shrink)/2, (WINY / 2) + 5);
			}			

			//X axis line
			sf::Vertex linex[] =
			{
				sf::Vertex(sf::Vector2f(0,(WINY / 2)),sf::Color::White),
				sf::Vertex(sf::Vector2f(WINX, (WINY / 2)),sf::Color::White),
			};
			
			//Y axis line
			sf::Vertex liney[] =
			{
				sf::Vertex(sf::Vector2f((WINX / 2), 0),sf::Color::White),
				sf::Vertex(sf::Vector2f((WINX / 2), WINY),sf::Color::White),
			};

			// main loop of the program, which is running while window is opened
			while (window.isOpen())
			{	
				sf::Event event;

				//event catcher 
				while (window.pollEvent(event))
				{
					//close window if X button is pressed
					if (event.type == sf::Event::Closed)
						window.close();
				}

				//resets the window from previous frame
				window.clear(sf::Color::Black);

				//draws stuff in the window
				window.draw(function);
				window.draw(linex, 2, sf::Lines);
				window.draw(liney, 2, sf::Lines);
				window.draw(xmark);
				window.display();
			}
	}
	
	return 0;
};


