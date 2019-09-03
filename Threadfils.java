import java.awt.image.BufferedImage;
import java.io.File;
import java.io.IOException;

import javax.imageio.ImageIO;

public class Threadfils extends Thread{
	private int height,width,max,black,rowmax,rowmin,colmax,colmin;
	private int[] colors;
	public BufferedImage image;
	
	// constructeur de la classe Threadfils
	public Threadfils(int cmax,int cmin,int rmax,int rmin,int h,int w,int m,BufferedImage im,int[] col,int b) {
		this.height=h;
		this.width=w;
		this.black=b;
		this.image=im;
		this.colors=col;
		this.max=m;
		this.colmax=cmax;
		this.colmin=cmin;
		this.rowmax=rmax;
		this.rowmin=rmin;
	}
	
	public void run() {
		// le thread va travailler dans une partie du plan donnée par les limites rowmin , rowmax, colmax, colmin
		for (int row =rowmin; row < rowmax; row++) {
            for (int col =colmin; col < colmax; col++) {
                double c_re = (col - width/2)*4.0/width;
                double c_im = (row - height/2)*4.0/width;
                double x = 0, y = 0;
                int iteration = 0;
                while (x*x+y*y < 4 && iteration < max) {
                    double x_new = x*x-y*y+c_re;
                    y = 2*x*y+c_im;
                    x = x_new;
                    iteration++;
                }
                if (iteration < max)
                	//le cas où la suite U diverge pour ce point (x,y) c a d il existe une distance de > 4 
                	image.setRGB(col, row, colors[iteration]);
                else 
                	// la suite converge pour ce point et donc le point appartient à l'ensemble de Mandelbrot
                	image.setRGB(col, row, black);
            }
        }
		// le dernier thread enregistre la valeur finale de "image" dans le fichier mandelbrot.png
		
		if(this.getName()=="Thread-99") {
        try {
			ImageIO.write(image, "png", new File("mandelbrot.png"));
		} catch (IOException e) {
			e.printStackTrace();
		}
		}

	}
}
