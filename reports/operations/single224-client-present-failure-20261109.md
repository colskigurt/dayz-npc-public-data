# 클라이언트 CPU·Present 수집의 실패 경계

## 1. 작업 목표
서버CPU감소2쌍 결과에 이어 동일64개체씬의 클라이언트CPU와 실제Present를 기존 통합실행기로 비교할 수 있는지 확인한다. 서버틱을화면프레임으로대체하지않는다.
## 2. 실제 수행
기존npc-test-launcher와PresentMon2.6.0을재사용하여4회×30초 설정을검증했다. 처음PowerShell실행경로를잘못지정해실행되지않았고,nullPID영수증을실행성공으로인정하지않았다. 실제바탕화면바로가기경로로고친후새관리자수집기의권한·설정·실제PID를확인했다. UAC버튼자동조작없음. 첫5파트64개체조건을실행했다. 클라이언트전체CPU샘플러와Present수집을서버CPU창에연결했다.
## 3. 측정 결과
관리자준비완료,실제320렌더/64수신체및활성상태확인. 첫PresentMon은종료코드0이나csvCreated=false,CSV없음. stdout에는Started recording/Stopped recording,stderr에는유의한오류문구없음. 따라서Present·CPU Busy·GPU·표시분위수는모두미측정이다.
클라이언트전체CPU샘플러는초기소유검사에서거절됐다. 실제프로세스StartTime은영수증과맞았고Path조회는null이었다. TotalProcessorTime호출이전이므로CPU읽기API자체가불가능하다고판정하지않는다.
기존서버CPU독립2쌍감소26.8092%/25.9961%(중앙26.4026%)는이전별도근거로유지한다. 이번실행은유효한클라이언트비교에포함하지않는다.
## 4. 원인 분석
확인:기존전체CPU샘플러는실제경로의완전일치를필수로하므로경로null에서중단한다. 이실패와PresentCSV부재는별개의단계다. 관리자권한확인과종료코드0만으로실제표시이벤트를확보했다고판정할수없다.
추정/모름:경로null의권한/보호원인,PresentCSV미생성의이벤트전달/대상필터/도구내부원인은미확인이다. BattlEye차단이나엔진문제로단정하지않는다. 종료코드만보고도구성공으로취급하는가설은이번사례에서기각한다.
## 5. 검증 수준
설정·관리자실행진단성공,실제Present/클라이언트CPU측정실패.4회계획중첫조건만실행했고나머지는미실행이다. 기능·시각·성능·최종수용없음. 기존서버CPU기각스크린반복통과의범위를확대하지않는다.
## 6. 변경 사항
게임코드/PBO수정없음. 기존서명검증된후보그대로사용. 기존통합실행기코드수정없음. 설정포인터만새시험으로선택했고이전포인터원문/해시를보존했다. 새바탕화면실행기없음.
격리client-cpu-reader1은원본CPU샘플러를보존한후소유영수증해시/PID/생성시각/프로세스명/CIM시각/Steam요청/디스크바이너리해시를교차검증하도록준비했다. 조회가능한경로·프로필모순은거절한다. 실제프로세스경로가없으면확인불가로기록하며완전실행경로증명을주장하지않는다. CPU·QPC계산은기존방식. PowerShell구문검사만통과했고실행미검증이다. 보안설정변경·후킹·네이티브메모리쓰기없음.
## 7. 원시 증거
ANIM-single224-client-present1-cost64-screen1-five.
client-present1/admin-ready.json,admin-process-launched2.json,admin-launch-path-correction.json,admin-error-20261109-165042-597.json,configuration-selection.json,experiment-contract.json.
capture-0/consumed.json,tool-started.json,closed.json,stdout.log,stderr.log.
cost64-screen1/five/client-present-terminal.json,server-terminal.json,client-closed.json및원시CPU/상태로그.
client-cpu-reader1/contract.json,sample-owned-client-cpu.ps1. 첫잘못된실행의nullPID자료도보존한다.
## 8. 잔여 문제
다음은격리된소유클라이언트CPU읽기후보의실행가능성을확인하는최소시험이다. 실제프로세스경로미확인이라는신원증명한계를별도표시한다. Present는원시이벤트확보경로를확인하기전동일4회시험을반복하지않는다. 이번수집기·PresentMon·소유서버/클라이언트는종료됐다. 실패설정은재실행시기록보존가드로거절되므로다음시험은새실행ID가필요하다. 전투ON기능차이와64전체회귀/시각품질은별도미수용상태로유지한다. 20ms목표달성주장없음.
