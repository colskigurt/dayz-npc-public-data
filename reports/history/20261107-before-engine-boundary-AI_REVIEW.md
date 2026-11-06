# 단일 비용쌍과 반복 필요

셀 풀64 단일쌍:Present p95 OFF20.94/ON18.54ms,CPU20.80/18.38ms. 약11~12% 낮았으나 p50악화·동적작업량차이. screen통과이며재현성능/채택미완료.

[상세](reports/operations/grid-cell-pool-cost64-pilot-20261107.md) · [JSON](api/public/grid-cell-pool-cost64-pilot.json) · [근거](evidence/grid-cell-pool-cost64-pilot-20261107.txt)

ON/OFF 둘다64논리·320표시·64receiver·같은카메라/미션. receiver방향생략은양쪽OFF. CSV exit0·전경·전원생존·동일PresentMode/Sync0검증. FALL관측행1/50,low/ground분포차이가있어할당의순수CPU절감률로해석금지. 서버exit−1은정상종료미수용. 다음OFF/ON,ON/OFF 2쌍은아직미실행이며모든쌍을보고한다. 단일유리한결과로채택금지.
