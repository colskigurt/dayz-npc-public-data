## 1. 작업 목표

기존 network 8파트 후보를64NPC/512parts로 확장한 기능 시험을 감사하고,전이·복구·종료 상태와 서버 갱신 간격을 구분한다. 직전의64/64 기능 통과 표현을 정정한다. 이동 milestone64/64와 VALID_TERMINAL64/64는 사실이나 엄격한 분석 게이트는false다. 기존 종료 판정이나 물리 검사를 완화하지 않는다.

## 2. 실제 수행

기존 서명 후보765E와 기존5PBO를 유지한 격리 진단 server/client 1회다. 원래 인식·경로·Drive/Displace·접촉·지지·낙하·착지·재등반을 유지했다. 동일 NPC 내부28쌍만 차단하며 타NPC/플레이어/월드 충돌을 끄지 않았다. 기존8시험의 플레이어 자극·fixture를64개체로 확대했다. 각ID의 낮은지붕→높은지붕→지면복귀→재등반 이력을 누적하고,최종 FOLLOW/실제 Floor 지지 결과를 퇴역 전에 기록했다. 같은 좁은 지붕 동시 점유를 요구하지 않았다.

추가 기록은 서버 Mission OnUpdate timeslice와1Hz의 파트별 script NativeSeconds/AnimUpdates 차이다. 새 native hook·pose getter·영상은 없다. 이번 후속 감사는 종료된 TSV/JSON만 읽었고 새 게임 실행/게임 코드/PBO 변경을 하지 않았다. 8시험의 레거시 EIGHT 로그 이름은 유지됐지만plan/mission/terminal64행이 실제 규모의 근거다.

## 3. 측정 결과

|항목|sixtyfour1 단일 실행|
|---|---|
|논리NPC/native parts|64/512|
|active 구간|256.627초|
|누적4milestone/VALID_TERMINAL|64/64 및64/64|
|엄격한 분석 게이트|false;최종 failure 필드2개체 잔존|
|기록된 FALL→LAND→FOLLOW|94/94|
|전이 체인/복구 generation/최종 phase 정합 오류|0/0/0|
|서버 Mission timeslice p50/p95/p99|44/66/81ms|
|최대 Mission timeslice/>50ms|124ms/1593행;총6005행|
|NPC내8파트 script시간/업데이트 최대차이|0초/0회;요청 불일치0|
|Present/CPU busy/GPU/할당/GC|이번 실행 미측정|

server timeslice는 클라이언트 표시 프레임이나 함수 self-time이 아니다. 기존 평지 Present p95와 직접 비교하지 않는다. script 시간의벽시계비는약0.999832이나 actual graph clock/visible pose 속도의 증거가 아니다. TickCount의 signed32 wrap1회는인접차를modulo2^32로복원해처리했고원본값을보존했다.

## 4. 원인 분석

ID8:243.893초 지지소실→244.319LAND→244.752FOLLOW. 최종5초 이동거리 합0.491m,접촉 거부증가0,ARRIVED가관찰됐다. CONTACT_SUPPORT_LOST 문자열이최종에도남았다는것만으로현재정체라고할수없다.

ID55:247.616FALL→248.158LAND→251.505EGRESS→256.571EGRESS_STALL/FOLLOW. 최종5초 표본상위치변화0m,contactSolve82회증가/거부78회증가,관절·월드거부NATIVE_JOINT_WORLD_1/3. 최종area는ground다. 마지막한프레임FOLLOW/Floor통과는경로추적재개를입증하지않는다. 이개체는종료시해소여부가확인되지않은정체로보존한다.

historicalFailureTransitions의CONTACT_SUPPORT_LOST94/CLIMB_EXIT_VALIDATION7/EGRESS_STALL16행은failure필드가여러전이에지속될수있으므로독립사건횟수가아니다. 94회FALL중각원인의수를그표로대체하지않는다. 모든낙하가타NPC에의한밀침이라는증거도없다.

검토한 기존 소스에서는 EGRESS 5초 정체 후 연결 차단·권한 해제·3초 재시도 대기·FOLLOW 복귀가 가능하다. CompleteEgress는failure문자열을항상지우는방식이아니다. 소스해석과실제로그를구분하며이번감사만으로모든packed기본멤버의완전동일성을새로인증하지않는다. failure문자열을지우거나검사를삭제해게이트를통과시키지않는다.

## 5. 검증 수준

64개체 이동이력/명령퇴역:단일 관찰 성공. 엄격한 기능 회귀:미통과. ID8 과거실패와ID55 종료직전접촉정체 구분:진단 성공. 8개체2회통과기록은그대로보존한다. 64개체반복검증/모든정체해소/실제애니메이션품질/비용개선/20ms p95/최종수용은미완료다.

## 6. 변경 사항

격리64mission·런처·기록기및분석스크립트만추가했다. 이번감사는audit_scale_terminal.py와보고파생본이다. 새PBO·시계·그래프·solver변경은없어새서명대상도없다. 실행전기존6PBO해시·서명검증기록을유지한다. 기존network PBO SHA=765E61B0B2E20EA3C7977FF17316808D99D8CDD94DDFBCE7A95F8829CAD6EED8.

서버DONE 후harness 강제정리,클라이언트forcedfalse/exit-1이며normalQuitVerified=false다. 런처프로세스exit0을게임정상종료로해석하지않는다. 운영서버/Steam/원본자산/기존보고서는변경하지않았다.

## 7. 원시 증거

sourceRunId:animation-network-role-20261101-1600/eight-part-20261102/network-gameplay/sixtyfour1. plan/mission/launch·RPT/script·gameplay-eight/milestones/native-state-contract/terminal-state/server-frame/part-command-clock TSV·scale-analysis/scale-timing-analysis/terminal-semantics-audit JSON을연결한다. evidence hash를다시대조하고종료된파일의안정성을검증한다. 공개packet은선정집계와ID8/55시간창·전이만포함하고원시전체/절대경로/PID/키/영상/게임자산은제외한다. 같은실행의추가분석을독립반복으로세지않는다.

## 8. 잔여 문제

다음은ID55의종료직전관절·월드접촉정체를기존fixture/회복replay로좁히는것이다. EGRESS_STALL후FOLLOW이름만으로성공시키지않고accepted movement·안전도착·후속재계획의실행을확인해야한다. 검사실패를강제성공시키거나정상밀침·낙하를제거하지않는다. ID8의이력필드와ID55의실제진행정체를같은해결책으로취급하지않는다.

네트워크8파트경로는채택보류다. 실제영상의시간연속성/일반클라이언트호환성/클라이언트와서버의분리비용이남았다. 실제Present와CPU/GPU측정은기존ETW권한·live worker를확인한뒤필요한비교만수행한다. scratch/SupportFast는별도후보이며이번자료에서성능개선을선언하지않는다. DaysGone은구체적적용질문이생길때만추가조사한다.

로컬 checkpoint SHA-256: 8846ddf548582f22c269e12482a38be1baa6d5630f7beca3f20dfe950baeba51
