
let allOptions = [
    { dx: 1, dy: 0 },
    { dx: -1, dy: 0 },
    { dx: 0, dy: 1 },
    { dx: 0, dy: -1 },
  ];
  
  let x1;
  let y1;
  let x2;
  let y2;
  let x3;
  let y3;
  
  let grid;
  let spacing = 10;
  let cols, rows;
  
  function make2DArray(cols, rows) {
    let arr = new Array(cols);
    for (let i = 0; i < arr.length; i++) {
      arr[i] = new Array(rows);
    }
    return arr;
  }
  
  function setup() {
    createCanvas(400, 400);
    cols = floor(width / spacing);
    rows = floor(height / spacing);
    xSeed = 5;
    ySeed = 20;
    seedSteps = 0;
    x1Steps = 0;
    x2Steps = 0;
    x3 = 30;
    y3 = 10;
    isFinishedPSeed = false;
    isFinishedP1 = false;
    isFinishedP2 = false;
    isFinishedP3 = false;
    background(51);
    grid = make2DArray(cols, rows);
    for (let i = 0; i < cols; i++) {
      for (let j = 0; j < rows; j++) {
        grid[i][j] = false;
      }
    }
  
    grid[x3][y3] = true;
  }
  
  function isValid(i, j) {
    if (i < 0 || i >= cols || j < 0 || j >= rows) {
      return false;
    }
    return !grid[i][j];
  }
  
  function drunk(x, y, preference) {
   
    point(x * spacing, y * spacing);
    let options = [];
    for (let option of allOptions) {
      let newX = x + option.dx;
      let newY = y + option.dy;
      if (isValid(newX, newY)) {
        //might need to tweak left pref
        if (preference == "not left more middle") {
          options.push(option);
          if (!(option.dx == -1 && option.dy == 0)) {
            options.push(option);
            options.push(option);
          }
          if (option.dx == 1 && option.dy == 0) {
            options.push(option);
            options.push(option);
          }
        }
        if (preference == "up")  {
          options.push(option);
          if (option.dx == 0 && option.dy == -1) {
            options.push(option);
            options.push(option);
          }
            
        }
        if (preference == "down")  {
          options.push(option);
          if (option.dx == 1 && option.dy == 0) {
            options.push(option);
            options.push(option);
          }
            
        }
      }
    }
    
    if (options.length > 0) {
      let step = random(options);
      
      strokeWeight(1);
      stroke(255);
      beginShape();
      vertex(x * spacing, y * spacing);
      x += step.dx;
      y += step.dy;
      vertex(x * spacing, y * spacing);
      endShape();
      
      grid[x][y] = true;
   
    } else {
      console.log(`I'm stuck!`);
      return [x, y, true];
    }
    return [x, y, false];
  }
  
  function draw() {
    
    strokeWeight(spacing * 0.5);
    point(xSeed * spacing, ySeed * spacing);
    if (seedSteps == 15) {
      x1 = xSeed;
      y1 = ySeed;
      grid[x1][y1] = true
    }
    if (seedSteps > 14) {
      stroke('blue');
      point(x1 * spacing, y1 * spacing);
    }
    
     if (seedSteps == 22) {
      x2 = xSeed;
      y2 = ySeed;
      grid[x2][y2] = true
    }
    if (seedSteps > 21) {
      stroke('red');
      point(x2 * spacing, y2 * spacing);
    }
    //point(x2 * spacing, y2 * spacing);
    //point(x3 * spacing, y3 * spacing);
    
    if (!isFinishedPSeed && seedSteps < 50) {
      if (seedSteps < 22){
        stroke(255);
      } else {
        stroke("yellow");
      }
      
      [xSeed, ySeed, isFinishedPSeed] = drunk(xSeed, ySeed, "not left more middle");
      seedSteps += 1;
    }
    
    if (!isFinishedP1 && seedSteps > 15 && x1Steps < 25) {
      
      [x1, y1, isFinishedP1] = drunk(x1, y1, "up");
      x1Steps +=1;
    }
    
    if (!isFinishedP2 && seedSteps > 22 && x2Steps < 25) {
      
      [x2, y2, isFinishedP2] = drunk(x2, y2, "down");
      x2Steps +=1;
    }
  
    
    if (isFinishedPSeed && isFinishedP1 && isFinishedP2) {
      noLoop();
    }
  
  }