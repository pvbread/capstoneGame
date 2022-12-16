
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
    x1 = 10;
    y1 = 10;
    x2 = 30;
    y2 = 30;
    x3 = 30;
    y3 = 10;
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
    grid[x1][y1] = true;
    grid[x2][y2] = true;
    grid[x3][y3] = true;
  }
  
  function isValid(i, j) {
    if (i < 0 || i >= cols || j < 0 || j >= rows) {
      return false;
    }
    return !grid[i][j];
  }
  
  function drunk(x, y) {
   
    point(x * spacing, y * spacing);
    let options = [];
    for (let option of allOptions) {
      let newX = x + option.dx;
      let newY = y + option.dy;
      if (isValid(newX, newY)) {
        options.push(option);
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
  
  // in p5js draw loops until we call no loop
  // so the idea is to only stop looping when all drunks are done
  // but at the same time, we can't ask a drunk to walk if he's
  // passed out so have to set guards for that
  function draw() {
    stroke(255);
    strokeWeight(spacing * 0.5);
    point(x1 * spacing, y1 * spacing);
    point(x2 * spacing, y2 * spacing);
    point(x3 * spacing, y3 * spacing);
    
    if (!isFinishedP1) {
      [x1, y1, isFinishedP1] = drunk(x1, y1);
    }
    if (!isFinishedP2) {
      [x2, y2, isFinishedP2] = drunk(x2, y2);
    }
    if (!isFinishedP3) {
      [x3, y3, isFinishedP3] = drunk(x3, y3);
    }
    
    
    if (isFinishedP1 && isFinishedP2 && isFinishedP3) {
      noLoop();
    }
  }