let centerX, centerY;
let triangleSize = 100; 
let halfSize = triangleSize / 2;
let time = 0;
let move = 0;
let angle = 0;
let phase = 0; 
let move1 = 0; 
let cutSpeed = 50;


function setup() {
    createCanvas(1280,720);
    background(0);   
    angleMode(DEGREES);
    centerX = width / 2;
    centerY = height / 2;
}

function draw() {
    background(0);

    stroke(255);
    fill('white');
    
    if (phase === 0) {
        
        if (time <= 60) {
            ellipseMode(CENTER);
            ellipse(centerX, centerY, triangleSize, triangleSize);
        }
        
        else if (time > 60 && time <= 120) {
            move += 1;  
            angle += 1; 

            // Top half-circle
            push();
            translate(centerX, centerY - halfSize - move);
            rotate(-angle / 10);
            beginShape();
            vertex(-halfSize, 0);
            bezierVertex(-halfSize, -halfSize, halfSize, -halfSize, halfSize, 0);
            vertex(halfSize, 0);
            endShape(CLOSE);
            pop();
	    

            // Bottom half-circle
            push();
            translate(centerX, centerY + halfSize + move);
            rotate(angle / 10);
            beginShape();
            vertex(-halfSize, 0);
            bezierVertex(-halfSize, halfSize, halfSize, halfSize, halfSize, 0);
            vertex(halfSize, 0);
            endShape(CLOSE);
            pop();

            // Left half-circle
            push();
            translate(centerX - halfSize - move, centerY);
            rotate(angle / 10);
            beginShape();
            vertex(0, -halfSize);
            bezierVertex(-halfSize, -halfSize, -halfSize, halfSize, 0, halfSize);
            vertex(0, halfSize);
            endShape(CLOSE);
            pop();

            // Right half-circle
            push();
            translate(centerX + halfSize + move, centerY);
            rotate(-angle / 10);
            beginShape();
            vertex(0, -halfSize);
            bezierVertex(halfSize, -halfSize, halfSize, halfSize, 0, halfSize);
            vertex(0, halfSize);
            endShape(CLOSE);
            pop();

            // Combine into a square
            push();
            rectMode(CENTER);
            translate(centerX, centerY);
            rect(0, 0, triangleSize, triangleSize);
            pop();
            
            // Cutting lines
            for (let i = 0; i < 4; i++) {
              push();
              translate(centerX, centerY);
              rotate(i * 90);
              line(-centerX - 100 + move * cutSpeed, -halfSize, -centerX + move * cutSpeed, -halfSize);
              pop();
            }
        } else {
          
            phase = 1;
            time = 0;
            move = 0;
            angle = 0;
        }
    } else if (phase === 1) {
       
        if (time <= 60) {
            rectMode(CENTER);
            rect(centerX, centerY, triangleSize, triangleSize);
        }
        
        else if (time > 60 && time <= 180) {
            move += 1; 
            angle += 1; 

            // Left triangle
            push();
            translate(centerX - halfSize / 2 - move, centerY - halfSize / 2 + move);
            rotate(-angle / 10);
            beginShape();
            vertex(-halfSize / 2, 1.5 * halfSize);
            vertex(halfSize / 2, -halfSize / 2);
            vertex(-halfSize / 2, -halfSize / 2);
            endShape(CLOSE);
            pop();

            // Right triangle
            push();
            translate(centerX + halfSize / 2 - move, centerY - halfSize / 2 - 2 * move);
            rotate(-angle / 3);
            beginShape();
            vertex(halfSize / 2, 1.5 * halfSize);
            vertex(-halfSize / 2, -halfSize / 2);
            vertex(halfSize / 2, -halfSize / 2);
            endShape(CLOSE);
            pop();

            // Combine into a large triangle
            push();
            beginShape();
            vertex(centerX - halfSize, centerY + halfSize);
            vertex(centerX + halfSize, centerY + halfSize);
            vertex(centerX, centerY - halfSize);
            endShape(CLOSE);
            pop();
                        // Cutting lines
                        push();
                        translate(centerX + move * cutSpeed, centerY - halfSize + move * cutSpeed * 2);
                        line(-400, -800, -300, -600);
                        pop();
            
                        push();
                        translate(centerX - move * cutSpeed, centerY - halfSize + move * cutSpeed * 2);
                        line(400, -800, 300, -600);
                        pop();
        } else {
            
            phase = 2;
            time = 0;
            move = 0;
            angle = 0;
        }
    } else if (phase === 2) {
        
        TriangletoCircle();
    }

  
    time += 1;
}

function TriangletoCircle() {
  push();

  translate(centerX, centerY - halfSize/2); 

  rotate(angle * angle); 

  if (angle < 30) { 
    beginShape();
      vertex(-halfSize, 0.5 * halfSize);
      vertex(halfSize, 0.5 * halfSize);
      vertex(0, -1.5 * halfSize);
    endShape(CLOSE);
  } else {
    ellipse(0, 0, halfSize*2, halfSize*2);

    // Top triangle
    push(); 

    translate(0, -0.75 * halfSize - move1);
    rotate(angle * angle);

    beginShape();
      vertex(-0.5 * halfSize, 0.25 * halfSize);
      vertex(halfSize / 2, 0.25 * halfSize);
      vertex(0, -0.75 * halfSize);
    endShape(CLOSE);

    pop();

    // Right triangle
    push();

    translate(0.75 * halfSize + move1, 0);
    rotate(angle * angle);
    
    beginShape();
      vertex(-halfSize / 4, 0.5 * halfSize);
      vertex(halfSize / 4, 0.5 * halfSize);
      vertex(-halfSize / 4, -0.5 * halfSize);
    endShape(CLOSE);

    pop();

    // Left triangle
    push();

    translate(-0.75 * halfSize - move1, 0);
    rotate(angle * angle);

    beginShape();
      vertex(-halfSize / 4, 0.5 * halfSize);
      vertex(halfSize / 4, 0.5 * halfSize);
      vertex(halfSize / 4, -0.5 * halfSize);
    endShape(CLOSE);

    pop();

    move1 += 5; 
  }
  angle += 0.5;

  pop();

  
  if (angle >= 60) {
  
    phase = 0;
    time = 0;
    move = 0;
    angle = 0;
    move1 = 0;
  }
}
