## 1. 작업 목표

기존64개체 EGRESS-only 시험에서 남은 ID24의 지면 입구 정체를 기존 기록으로 좁힌다. 이미 연구한16방향 탐색 후보와 중복을 먼저 확인하고, 현재 지지·접촉 판정을 유지한 작은 통제 시험으로 수정 후보를 선별한다. 애니메이션 시계/그래프와64개체 성능 시험은 별도로 유지한다.

## 2. 실제 수행

기존sixtyfour-egress1의 ID24 300.426/330.459/359.507초 root·heading·lift·idleOffset·4본 포즈를 frozen 하네스에 넣었다. getter를 기록 포즈 배열로 대체한 기존 방식이며, 실제 개체의 애니메이션·주변 개체·최종 상태 전체를 재현하지 않았다. 동일 root의3포즈는 독립적인3회 성능 반복이 아니다.

기존2026-10-30 candidate-refinement의 packed 소스 일치를 확인하고 원래8방향 뒤8중간 방향을 추가하는 방법을 재사용했다. default OFF/ON 및 phase/Active/EntryAligning/PoseClearance guard를 비교했다. 새 방향 알고리즘은 만들지 않았다.

기존 SDK SphereCastBullet과SphereOverlapBullet/Contact를 이용해 root 고정3몸통 구, 반지름0.23m, 원래 Walls mask의 정지·16방향·정/역방향을 관찰했다. 한 실행당147개 스윕과294개 endpoint 요약,5개 normal 방향 보정 관찰을 저장했다. fixture index·shape pair·모든 contact 깊이를 기록했으며, callback contact 참조를 다음 프레임으로 보관하지 않았다.

다음은 원래 PressureEscapeStepClear의 수식/반경/월드·지지·peer·예약 검사를 그대로 둔 채 frozen 첫 이동·연결 검사 거리만3cm에서1cm로 바꿨다. 이어 원래 PlanPressureEscape+PressureSlide에0.05/0.016666667 입력을 제공해 거리 효과를 비교했다. 이 입력은 격리된 공간 하네스에만 전달했고 실제 native 애니메이션 시계를 바꾸지 않았다. synthetic Now는0.05초씩 증가하며 case당 최대200호출·0.3m 이동을 진단 한도로 사용했다. 정적1cm 이동을 곧바로 복구 성공으로 간주하지 않았다.

준비 실패도 보존했다. refinement1의 지역변수 중복, core-queries1의 진단 산술식 컴파일 실패, shortstep1cm의 symlink 실제 경로/alias 불일치로 인한 정확한 known-warning guard 거부, short-motor1의 함수 범위 endContacts 이름 중복은 후속 별도 폴더에서 각각 수정했다. guard 전체 허용·원시 로그 덮어쓰기는 없었다.

## 3. 측정 결과

|진단 항목|결과|판정 범위|
|---|---|---|
|원래4관절 정지 검사|3/3 통과|frozen 포즈의 일반 solver|
|기존16방향 계획 OFF/ON|각0/3 선택|이번3입력에서 방향 추가 효과 없음|
|기존3cm 복구 첫 이동|0/48 통과|3포즈×16방향|
|1cm 복구 첫 이동|6/48 통과|각포즈 ±Z 두 방향, 총6개|
|1cm 직선 검사 endpoint|6개 모두 첫1걸음 뒤 full static endpoint|그 자리는 원래 정지 검사도 통과하므로 탈출 증거 아님|
|원래3cm 계획+이동 연결|3case 모두0걸음·0m|200호출 후 NO_VALID_LOCAL_EXIT|
|1cm 계획+이동 연결|3case 모두2걸음·약0.0195315m 후 재정체|0.3m 진단 목표 미도달, 해결책 미수용|
|motor 최종4관절 검사|6/6 통과|고정 몸통 검사와의 범위 차이 잔존|
|body core1 정지 SphereCast|접촉 false|동일 시작/끝 구형 스윕|
|동일 core1 SphereOverlap|fixture0 접촉1개, native depth -0.0059416m|AABB gap은 +0.0548m, 구형 스윕과 의미 차이|
|normal 방향 +1/+2cm|deepest native depth -0.0158231/-0.0261401m|각2접촉, first contact만 보면 다른 결론 가능|
|normal 방향 -1cm|접촉0|이 위치의 구만 이동한 진단, 실제 NPC 이동 아님|
|normal 방향 -2cm|다른 fixture1 접촉1개|-0.00223702m, 더 멀리 이동하면 안전하다는 보장 없음|

이번에 새8/64 실제 이동 회귀, Present/CPU busy/GPU p50/p95/p99, 할당/GC, 시각 부드러움은 측정하지 않았다. 기존8/8 결과와64 높은면63/64 실패는 이전 보고서 그대로 보존한다. 20ms p95는 목표다. 원본RPT의 완료마커 반복을 독립 반복으로 세지 않는다.

## 4. 원인 분석

확인된 사실: 일반 이동 solver는4개 포즈 기반 관절을 검사하는 반면 복구 경로는 root에 고정된0.35/0.9/1.5m 높이의3개 구를 추가한다. 이번 위치는 일반 정지 검사에는 합격하지만 복구 첫 이동 대부분에는 불합격이다. fixed core1의 실제 시작 겹침은 stationary SphereCast와 보수적 bounding box 양의 gap만으로 식별되지 않았다. 이 차이는 API 의미·볼륨 계약의 차이이며 엔진 버그를 증명하지 않는다.

native contact normal 방향 이동은 이번 입력에서 실제 측정해 해석했다. +normal은 deepest 음수 깊이가 커지고 -normal1cm는 기존 접촉에서 빠지지만 -normal2cm는 다른 컨테이너에 닿는다. 첫 contact와 가장 깊은 contact가 다르고 같은 fixture/shape pair에서 여러 점이 나오므로, firstDepth만 보거나 object+shape tuple만으로 새 접촉을 전부 구분할 수 있다고 가정하는 방식은 안전한 fallback 근거가 아니다. native signed depth를 기존 AABB clearance와 혼동하지 않는다.

가설: 포즈 기반 일반 이동과 root 고정 복구 몸통의 허용 공간 불일치가 정체를 유지하는 원인 후보다. 고정 몸통의 과거 벽 통과/낙하 방지 계약을 확인하기 전에는 어느 쪽이 잘못됐다고 단정하지 않는다. 포즈가 바뀌는 실제 상황에서 이 불일치가 해소되는지, 접근 정렬이 탈출 회전을 되돌리는지는 이번 frozen 시험으로 확정하지 않았다.

효과 부족: 방향16개 추가와 걸음1cm 축소만으로는 이 입력을 해결하지 못했다. 6개의 첫 이동 통과만 보고 작은 걸음 후보를 배포하지 않는다. 겹침 판정을 무조건 참으로 만들거나 collision radius를 축소하거나 cache를 재사용할 근거도 없다.

## 5. 검증 수준

ID24 정적 공간 원인 구분·SDK 실제 접촉 관찰: 진단 성공. 기존 방향 후보 및 단순 short-step 해결책: 이번 입력 범위에서 효과 부족/미수용. 기능 회귀·시각 품질·반복 성능 개선·최종 수용: 미완료. 3포즈는 같은 root를 공유하며 독립 반복 검증이 아니다. 전체 골은 active다.

## 6. 변경 사항

새 격리 mission·staging/analysis·선정 TSV/JSON/보고서만 추가했다. 게임플레이 소스·PBO·ANM·그래프·native 시계·운영 서버·Steam 게시본은 바꾸지 않았다. 새PBO가 없어 새 서명 대상은 없고, 실행 전 기존6PBO 해시·서명 검증은 성공했다. 개인키는 공개하지 않는다. 완료 후 owned server를 하네스가 정리한 것으로 normalExitVerified=false이며 게임 정상 종료로 바꾸지 않는다.

## 7. 원시 증거

실행 묶음은 network-gameplay/{actor24-refinement1/2,actor24-core-queries1/2/3-identity,actor24-shortstep1cm/1cm2,actor24-short-motor1/2}. sourceRunId·mission hash·plan·정확한 warning guard·server launch·RPT/script·frozen/core/overlap/calibration/motor TSV를 로컬 무결성 목록에 연결한다. actor24-overlap-diagnosis-v2.json은 기존 v1을 보존한 파생 분석이다. 공개 packet에는 선택 집계와 자체 복구 함수 발췌만 제공한다. 원시 전체·영상·게임 자산·로컬 절대경로·프로세스 정보·키는 노출하지 않는다. 모든 대상 실행의 terminal 기록과 살아 있는 시험 프로세스 부재를 확인한 뒤 파일 안정성·JSON·이중 해시 검증 후 저장한다.

## 8. 잔여 문제

가장 가치 있는 다음 산출물은 고정 root 몸통 검사의 도입 이유와 과거 벽/가장자리 실패 사례를 연결한 복구 볼륨 계약이다. 후보가 필요하면 supported FOLLOW만 대상으로 pose-aware 검사와 기존 fixed core의 차이를 국소 비교하고 FALL/LAND 지지·스윕과 정상 밀침·낙하·재등반을 유지한다. 예상 이점은 잘못된 복구 막힘 완화이며, 위험은 벽 통과·접촉 깊어짐·추가 질의 비용이다. 먼저 이번 ID24와 과거 침투 실패 입력에서 비교하고 통과한 후보에만8→64를 수행한다. 해결 후보 없이 같은64시험을 반복하지 않는다.

물리 비용과 애니메이션 품질은 여전히 별도 과제다. 이번에 추가 overlap 질의를 제품의 매틱 경로로 넣지 않았고 비용 절감을 주장하지 않는다. 현재 개선된 network 미리보기의 사용자 관찰은 보존하되 전체14클립/64개체의 최종 수용으로 확대하지 않는다. 실제 표시 비용 ETW 비교는 기존 사용자 관리자 승인 경로가 필요하며 이번에는 요청/실행하지 않았다.

로컬 checkpoint SHA-256: df5f2dd5c004c4defb4ac7813a578ec82d511c195019c3bc1fc8018f2551a1ea
