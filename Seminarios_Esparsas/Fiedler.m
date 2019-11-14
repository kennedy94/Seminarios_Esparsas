function [p1, p2, v,d] = Fiedler(A)
  n = size(A, 1);
  if(n < 2)
    p1 = 1; p2 = 2; v = 1; d = 0;
    return;
  end
  opt.disp = 0;
  opt.tol = eps;
  
  S = A | A' | speye(n);
  
  S = diag(sum(S)) - S;
  
  [v,d] = eigs(S, 2, 'SA', opt);
  
  v = v(:, 2);
  d = d(2, 2);
  
  [ignore p1] = sort(v);
  p2 = flip(p1);
endfunction