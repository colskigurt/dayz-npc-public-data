# HIGH목표진단의 수용 경계

검증된5파트 유지. 새HIGH목표단일진단의Present p95는14.98ms였지만HIGH다수관측조건실패로미수용. 별도서버CPUquery union24.85%inclusive,스택누락26.17%. 새최적화없음.

[상세](reports/operations/high-five-diagnostic-20261106.md) · [TXT](reports/operations/high-five-diagnostic-20261106.txt) · [근거](evidence/high-five-diagnostic-20261106.txt) · [JSON](api/public/high-five-diagnostic.json)

64명·320파트·고정HIGH목표·구름/비/안개0. Present15초와CPU8초설정은약47초떨어진별도창이다. Present보수문맥HIGH비율46.875~71.875%로사전50%미통과. CPU요청/후스냅샷64HIGH는먼저측정한Present창을통과시켜주지않는다.

서버9854가중표본,질의union2449·누락2579·lost0. VMinclusive와합산금지,할당/GC/self-time미측정. 전체ETL은11.9초,분석10.9초라요청8초와구분한다. 새게임실행1회이며같은ETL재분석은반복아님.

같은시험통과까지반복금지. 다음은이미설계된상태제한SupportSweepFast의구현/보류지점과현재5파트코드차이대조. 옛roof절감/terrain회귀/기능실패를보존하고미검증활성화하지않는다.

[직전 안내](reports/history/20261106-before-high-five-AI_REVIEW.md)
