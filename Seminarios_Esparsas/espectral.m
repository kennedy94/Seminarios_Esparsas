function espectral(instancia)
  s = load(instancia);
  
  A = spconvert(s);
  A = A' + A;
  
  %A = spconvert(Problem.A);
  [m n] = size(A);
  tau = nnz(A);
  
  [p1, p2, v, d] = Fiedler(A);
  
  A1 = A(p1, p1);
  A2 = A(p2, p2);
  
  
  subplot(2,2,1);
  spy(A);
  title('Original')
  subplot(2,2,2);
  spy(A1);
  title('Fiedler')
  subplot(2,2,3);
  spy(A2);
  load esparsa_vdd_rcm.dat
  A3 = spconvert(esparsa_vdd_rcm);
  subplot(2,2,3);
  spy(A3);
  title('Cuthill Mckee Reverso')
  
  load esparsa_vdd_sloan.dat
  A4 = spconvert(esparsa_vdd_sloan);
  subplot(2,2,4);
  spy(A4);
  title('Sloan')
  
  
  %disp(envelope(A))
  %disp(envelope(A1))
  %disp(envelope(A2))
  %disp(envelope(A3))
  %disp(envelope(A4))
  
endfunction


function soma = envelope(A)
  
  n = size(A,1);
  e = zeros(n, 1);
  
  [ii, jj, ignore] = find(A);
  
  
  for t = 1: nnz(A)
    if((ii(t) > jj(t)) &&  (ii(t) - jj(t) > e(ii(t))))
      e(ii(t)) = ii(t) - jj(t);
    endif
    
    if ((jj(t) > ii(t)) && (jj(t) - ii(t) > e(jj(t))))
			e(jj(t)) = jj(t) - ii(t);
    endif
  endfor
 
  soma = 2*sum(e) + n;
endfunction