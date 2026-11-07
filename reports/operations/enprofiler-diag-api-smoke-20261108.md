# 공식Enforce 함수 프로파일러 — 진단서버 API 확인

## 1. 작업 목표

일반서버ETW의명령실행기표본을실제Enforce함수명으로좁힐지원경로가있는지확인한다. 동일ETL재수집이나새물리후보구현은하지않는다.

## 2. 실제 수행

설치SDK enprofiler.c와공식DayZ Diag Menu/Modding Basics문서를확인했다. SDK는개발/diag전용임을명시한다. 설치DayZDiag를-server로별도실행해NPC/모드없이알려진메서드50회만호출하고EnProfiler API로함수명·횟수·시간원시값을읽었다. API외네이티브메모리쓰기/후킹/권한설정변경없음.

## 3. 측정 결과

EnProfiler.IsEnabledC=true. NFEnProfilerProbe::Work의예상50회/직접조회50회/이름포함횟수테이블50회가일치했다. GetTimeOfFunc원시값25.27,단위미확인. ProfiledSessionFrames48이며즉시enable/disable로부분프레임이포함돼호출50회를프레임50개로해석하지않는다. NPC0,새PBO0,성능/표시p50/p95/p99미측정이다.

## 4. 원인 분석

확인:현재설치diag서버에서공식API가실제로함수이름과정확한알려진호출수를반환한다. 따라서익명네이티브실행기주소만계속분석할필요없이,별도diag장면에서스크립트소유자순위를얻는경로가있다. 아직실제NPC함수순위를얻은것은아니다.

SDK상proto메서드는별도추적되지않지만상위스크립트시간에포함된다. 이시간을exclusive/self-time이나일반release절대비용으로사용하지않는다. GetTimePerFunc전체배열/실제NPC부하의가용성과오버헤드는아직미검증이다.

## 5. 검증 수준

알려진단일함수API진단1회성공. 실제NPC호출자·8/64기능·시각·최적화수용은미실행이다. 첫시도는OnUpdate의인자명dt/timeslice불일치로컴파일실패했고API미실행. 해당소유프로세스만정상닫기시도후종료한기록을보존했다. 성공실행도exit−1/내장BunkerBroadcastManager누수경고가있어정상종료통과아님.

## 6. 변경 사항

격리미션/프로브/진단실행기록만추가했다. 첫미션을덮어쓰지않고인자명만수정한새실행을만들었다. raw출력의구분자가문자열backslash-t여서파생분석에서만정규화했다. 게임PBO·NPC·애니메이션·기존ETL·서명불변이며새서명대상없음.

## 7. 원시 증거

ANIM-enprofiler-api-smoke1/2. plan/mission/launch/terminal,첫실패cleanup,SDK해시,enprofiler-api/counts TSV,스크립트로그와analysis.json을연결한다. 공개에는함수프로브이름·집계·해시만제공하고로컬경로/프로세스/원시엔진파일은제외한다.

## 8. 잔여 문제

다음은현재NPC와같은스크립트/자극을사용하는격리diag장면에서한정된함수이름별분포를얻을수있는지준비하는것이다. 진단과일반서버의역할/부하동등성을먼저확인하며절대시간을일반서버개선율로옮기지않는다. 실제수정후의성능판정은일반서버의기존통제비교로해야한다. 이API시험을애니메이션해결/물리비용절감으로보고하지않는다.

공식근거: [DayZ 진단메뉴](https://community.bistudio.com/wiki/DayZ:Diag_Menu), [DayZ 모딩기본](https://community.bistudio.com/wiki/DayZ:Modding_Basics).
