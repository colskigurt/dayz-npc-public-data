## 1. 작업 목표

바닐라 개체당 자원 비용을 추적하고, 커스텀의 큰 비용이 단일 개체부터 나타나는지 또는 군집 밀집/호출 빈도 증가에서 나타나는지 구분한다. 전체 비용을64로 나누어 순수 개체 비용이라고 하지 않는다. 이미 수행한64실험은 해시 검증 후 재사용하고1/8만 추가한다.

## 2. 실제 수행

기존 matched-flat-pursuit 하네스/PBO/카메라/평지/플레이어 입력/205초 구간을 재사용했다. 새로운 알고리즘이나 애니메이션 속도·LOD·물리 검사 삭제를 적용하지 않았다. 새1/8씬은 개체 수 상수만 다르며 같은0→V→C→C연속→V순서다. 정상 종료/퇴역, target/이동, 프레임·CPU·GPU·script dt·기존 중첩 카운터를 검사했다. 새 모니터는 프로세스 private/working-set만 추가하며 메모리 할당/GC profiler가 아니다. ETW helper는 대상/생성시각/프로필/관리자/서명/해시를 검증했고 native hook은 없다.

설치 scripts.pbo의 실제 선언·명령 핸들러와 anims_cfg.pbo의 골격을 읽었다. 작업 드라이브 추출본은 바이트와 canonical lines가 설치본과 일치하지 않아 현재 근거로 인증하지 않았다. 골격 count는 설치 PBO의 uncompressed member에서95를 직접 파싱했다. 커스텀8개 compiled XOB는 현재 resource PBO와 원본 compiled file의 바이트 일치를 검증했으며 선언 본 합계535다. 처음 참고한250본 원형 skeleton3종은 실행8파트의535본과 다르므로 후자의 refined inventory를 사용한다. preliminary proof의 source-match hardcode오류는v2에서false로 정정하고 원문을 보존했다.

REA로 현재 diag executable SHA를 확인했다. Ghidra는 가용하지만 전체 자동 분석을 게임 측정과 병행하지 않았다. REA objdump symbols 경로는 ERR_REA_WINDOWS_NATIVE로 거부됐고 설치/설정 변경은 하지 않았다. 기존 같은 바이너리의 animation map과 PE RTTI parser를 재사용해 Animal/Infected의 primary code-pointer prefix369개 중358개 동일 ordinal target을 확인했다. 차이가 있는slot365 두 함수만 unwind 범위 내 Capstone으로 읽었다. 완전 네이티브 함수 추적·runtime self-time을 확보했다는 뜻은 아니다. REA세션은 종료했다.

## 3. 측정 결과

첫7초/마지막5초 제외, 약33초 창이다. 새1/8은 각각 독립1회이며 C2/C3는 같은 개체의 연속 창이다. 이전64는 독립2회다. 같은 입력 recipe지만 물리 궤적/밀집도/실행 시각·주변 부하는 완전 동일하지 않다.1/8/64를 같은 시간의 독립 반복처럼 합치지 않는다.

| 수/실행 | 바닐라 Present p95 ms | 커스텀 Present p95 ms | 바닐라 CPU busy p95 ms | 커스텀 CPU busy p95 ms | 커스텀 CPU 시계/게임초 |
|---|---|---|---|---|---|
|1/1|22.736~23.094|23.081~23.785|9.322~9.990|9.583~10.597|0.833~0.833|
|8/2|22.823~23.220|25.662~27.915|8.794~10.660|12.745~14.483|0.833~0.834|
|64/3|24.549~24.734|67.605~72.382|13.671~13.790|67.426~71.401|0.742~0.764|
|64/4|24.172~24.619|67.879~71.782|12.592~13.342|67.370~69.981|0.740~0.757|

각 수에서 바닐라/커스텀 모두 모든 개체가 목표를 인식하고2m이상 이동했다. 이것은 평지 추격 검증이며 전체 terminal/등반/전투 회귀가 아니다. CPU busy는 전체 프레임 busy, processCPU는 스레드 합계라1000ms/벽시계초를 넘을 수 있다.

아래는 커스텀 집계값을 논리 개체 수 또는 실제 호출 수로 나눈 평균이다. 특정 개체의 독립 self-time이 아니며 중첩 경과 범위를 합산하지 않는다. foot 범위/ray수 역시 반복문·bookkeeping을 포함하여 RayCastBullet 자체 비용이라고 하지 않는다.

| 수/구간 | 평균 solve/개체·초 | solve/leader callback | 평균 ray/개체·초 | 접촉 범위 ms/개체·초 | 접촉 범위 ms/solve |
|---|---:|---:|---:|---:|---:|
|1/2|16.6517|1.0000|166.5168|2.1127|0.1269|
|1/3|16.6518|1.0000|166.5182|2.0759|0.1247|
|8/2|32.1697|1.9282|321.6966|3.1692|0.0985|
|8/3|31.7101|1.9034|317.1011|3.0853|0.0973|
|64/2|30.5393|1.9992|305.3927|3.1486|0.1031|
|64/3|29.6694|2.0001|296.6943|2.8478|0.0960|
|64/2|30.2762|1.9998|302.7619|3.2707|0.1080|
|64/3|29.6118|1.9998|296.1176|2.9730|0.1004|

메모리:1실행 private median은 empty약5899MiB→V1약5912MiB→C1약5943MiB→V1복귀약5950MiB였다.8실행은 empty약5895→V8약5935→C8약5954→V8복귀약5957MiB였다. 자산 lazy load/cache/heap retention이 포함되고 population을 줄여도 돌아오지 않으므로 개체당 RAM으로 나누지 않는다. 두64실험에는 이 메모리 계측이 없어 소급 보간하지 않는다.

## 4. 원인 분석

확인:1마리에서는 커스텀 전체 CPU가 바닐라보다 명확하게 비싸다고 구분되지 않았다.8에서 차이가 나타나고64에서는 기존의 큰 프레임/CPU 지연이 확인된다. 절편·배경 부하·VSync·캐시·갱신 빈도가 섞여 있으므로1마리의 순수 CPU가격이나 선형 개체당 단가를 확정하지 않는다. processCPU 기준으로도1마리 C<V 창과 V 반복의 변동이 있어 미세 차이를 개선으로 해석할 수 없다.

확인:1개체의 contact solve는 leader callback당 약1회,8/64에서는 평균 약1.9~2회다. 설치된 자체 PBO의 일반 DriveGround→SolveContacts 및 Crowd.Displace→SolveContacts 호출 지점이 추가 호출 후보와 연결된다. 현재 counter는 호출자별 분리가 없어 정확히 모든 추가 호출이 그 경로라고 확정하지 않는다. flat FOLLOW에서도 밀침에 따른 contact 재평가가 가능한 구조다. 밀침/발 지지 안전 검사를 삭제하거나 같은 결과라고 가정해 재사용하지 않았다.

관찰:contact solve당 평균 경과가 개체 수에 비례해 폭증하지는 않았다. 큰 군집에서는 호출량·추가 접촉 경로·전역 추적·8파트 native 실행이 함께 누적되는 후보를 우선 본다. 바닐라1instance/95선언본, 커스텀8instances/535선언본은 구성 차이다. 실제 active bone/LOD/skin/graph 평가 비용을 뜻하지 않으며535/95를 성능 배수로 쓰지 않는다. 이전 부분 골격 축소가 이미 적용된 상태라 본 수를 다시 줄이는 제안을 기존 결과 없이 반복하지 않는다.

시계:커스텀1마리에서도 CPU animation time≈0.833초/게임초,16.65Hz였다.8도≈0.833,64는≈0.74~0.76이다. 따라서 기존 시간 전달 문제를64CPU 포화만으로 설명하는 가설은 부족하다. callback/CPU 시계는 화면 mesh의 표시 시간/부드러움과 다르다. 바닐라의 동일 native clock ratio는 측정하지 않아 custom만의 엔진 시계 문제라고 단정하지 않는다.

미확인:바닐라 발 지지·관절 충돌·graph evaluation·blend·skin·upload·GC 각 self-time과 개체별 메모리. script에 native 명령 경계는 있으나 함수 이름에 전체 내부 범위를 귀속할 근거가 없다. RTTI shared targets만으로 같은 실행 모드/애니메이션 경로/비용이라고 할 수 없다.

## 5. 검증 수준

진단 성공:실제1/8 추격·종료/퇴역과 ETW·기존 카운터·process memory 확보, 기존64자료 hash 재인증, 설치 resource/자체 packed source 대조. 새1/8의 독립 반복 검증 완료는 아니다. 기능 통과는 flat chase에 한정한다. 시각적 애니메이션 수용·개별 native selftime 귀속·20ms 성능 목표·memory cleanup 최종 수용은 미완료다.

## 6. 변경 사항

기존5개 PBO는 그대로 사용했고 매 실행 전 해시·서명을 검증했다. 새 PBO/게임 알고리즘/운영 서버/배포물 변경 없음. count-only scene/launcher, process memory observer, 파생 분석·보고서만 추가했다. 새 빌드 식별은 기존729f3b bridge 및93507d gameplay다. 개인키 배포 없음. 클라이언트/PresentMon 모두 종료했고 강제 client 종료 없음. 기존 diagnostic 경고와 PGFilter의 개체 수에 맞는 종료 누수 경고는 보존했다. 실행 중 순수 누수량이나 GC량으로 해석하지 않는다.

## 7. 원시 증거

matched-flat-pursuit-20261102/per-agent의n1/n8에 실제205초 실행 원문을 보존한다. launch/admin/host-observations/client-frames/vanilla-agents/custom-agents/custom-costs/stages/mission/script/RPT/analysis, refined asset inventory/installed skeleton proof-v2/shipped boundaries-v2/packed contact sites/native class boundaries를 연결한다. 과거64 checkpoint와 analysis hash를 대조했다. 새로운 원문/이전 보고서를 덮어쓰지 않고 저장 전 파일 안정성·JSON·해시를 검증한다. 공개는 집계와 선정 경계만, raw 전체/설치 자산/로컬 경로/프로세스 식별자는 제외한다.

## 8. 잔여 문제

우선 연구1:기존 하네스에 호출자 ID만 부가한 격리 관측으로 locomotion/crowd/회복의 SolveContacts 횟수·경과를 분리한다. contact ON/OFF를 곧바로 최적화 후보로 적용하지 않는다. 동일 인자/포즈/시간/지지 객체가 같은 검사인지부터 확인하고 가장자리·낙하·피격·착지 시 무효화 조건을 보호한다.

우선 연구2:이미 확보한 부분 골격을 유지한 채8파트의 native scheduling/evaluation/render-facing work를 단일개체에서 분리한다. 확인된 entry/exit와 객체 ID로 좁게 계측하고 observer OFF/ON을 비교해야 한다. 바닐라와custom의 공통 native 경계가 selftime을 모두 포함한다고 가정하지 않는다. engine profiler 또는 검증된 native function attribution이 필요하며 현재 objdump 제한은 해당 경로에만 한정한다.

우선 연구3:같은 count에서0↔1↔0 및0↔8↔0 baseline을 교대로 반복해 바닐라 순수 증분 비용의 노이즈/드리프트 경계를 좁힌다. 작은 신호가 반복 오차보다 작으면 숫자를 확정하지 않는다. 원시 로그 용량은 제한하고 기존 manifest/보고 체계를 재사용한다. 실제 표시 포즈의 시간 연구는 별도로 유지하며 새 보간·시간 배율·업데이트 생략을 근거 없이 적용하지 않는다.

로컬 checkpoint SHA-256: bcd68925c79e92dbb55a6bfccc917cd8f8d1ab056f355c7ca5cf69a01d9714a8
