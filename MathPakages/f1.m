function y = f1(x, d)
  y = (d ^ 3)* cot(x).* sqrt (sin(x).^4 - cos(x).^4)
  if y < 0
    g = 0
  else
    g = 1
  endif

  endfunction

