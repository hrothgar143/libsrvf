% Computes preimages of sample points xv under the non-decreasing 
% piecewise-linear function T,F.
%
% Inputs
% T,F:  the function.  F must be non-decreasing.
% xv:   the sample points.  Must be non-decreasing.
%
% Outputs
% xvi:  preimages of xv
function xvi = plf_preimages( F, T, xv )
  xvi = zeros(1,length(xv));
  Fidx = 1;
  for xvidx = 1:length(xv)
    while( Fidx < length(F)-1 && xv(xvidx) > F(Fidx+1) )
      Fidx = Fidx+1;
    end

    dF = F(Fidx+1) - F(Fidx);
    if ( dF > 1e-4 )
      w1 = (F(Fidx+1) - xv(xvidx)) / dF;
      w2 = (xv(xvidx) - F(Fidx)) / dF;
      xvi(xvidx) = w1 * T(Fidx) + w2 * T(Fidx+1);
    else
      xvi(xvidx) = T(Fidx);
    end
  end
end


%!function v=_random_increasing_vector(v0,n)
%!  v=zeros(1,n);
%!  v(1)=v0;
%!  for i=2:n
%!    v(i)=v(i-1)+rand();
%!  end
%! end
%!
%!test
%! F=linspace(0,1,5);
%! T=linspace(0,1,5);
%! xv=linspace(0,1,100);
%! xvi=plf_preimages(F,T,xv);
%! assert(xvi,xv,1e-5);
%!
%!test
%! F=[0 0 1/2 1/2 1 1];
%! T=linspace(0,1,6);
%! tv=linspace(0,1,100);
%! xv=[0 0.2 0.499 0.5 0.501 0.999 1];
%! xvie=[0 0.28 0.3996 0.4 0.6004 0.7996 0.8];
%! xvi=plf_preimages(F,T,xv);
%! assert(xvi,xvie,1e-4);
%!
%!test
%! F=_random_increasing_vector(-100*rand(),50);
%! T=_random_increasing_vector(-5*rand(),50);
%! tv=linspace(T(1),T(end),100);
%! xv=interp1(T,F,tv);
%! xvi=plf_preimages(F,T,xv);
%! assert(xvi,tv,1e-4)
%!
%!test
%! F=_random_increasing_vector(0,1000);
%! F=F/F(end);
%! T=linspace(0,1,length(F));
%! tv=linspace(T(1),T(end),2000);
%! xv=interp1(T,F,tv);
%! xvi=plf_preimages(F,T,xv);
%! Fxvi=interp1(T,F,xvi);
%! assert(Fxvi,xv,1e-4)
