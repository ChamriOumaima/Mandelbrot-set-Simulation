import java.awt.Color;
import java.awt.image.BufferedImage;

public class mandelbrot {
	public static void main(String[] args) throws Exception {
        int width = 3000, height = 3000, max = 1000;
        BufferedImage image = new BufferedImage(width, height, BufferedImage.TYPE_INT_RGB);
        int black = 0;
        int[] colors = new int[max];
        for (int i = 0; i<max; i++) {
            colors[i] = Color.HSBtoRGB(i/256f, 1, i/(i+8f));
        }
        // création d'un tableau de 100 threads
        Threadfils[] t = new Threadfils[100];
        
        // initialisation de chaque threadfils
        for(int i=0;i<100;i++) {
        	t[i]=new Threadfils(30*(i+1),30*i,3000,0,height,width,max,image,colors,black);
        	
        }
        
        // lancement des threads en paralel
        for(int i=0;i<100;i++) {
        	t[i].start();
        	
        }
    }
}