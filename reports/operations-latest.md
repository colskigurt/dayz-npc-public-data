# 서버 파트 포즈 소비자 검토 — 루트 조회 국소 후보·미구현

작성: 2026-11-06T08:58:52.986055+09:00

## 1. 작업 목표
검증된5파트 기준을 유지하면서, 서버의 비대표 파트에서 안전하게 줄일 수 있는 실행 소유자가 있는지 기존 자료로 판단한다.4파트 비용3쌍의 불일치를 새 기능 개발로 우회하지 않는다.

## 2. 실제 수행
완료6회 계획에 핀된 PBO 합집합15개의 SHA256과 저장된 Enforce 소스 멤버를 직접 읽었다. 기본 dg_lab_inputs의18개 스크립트는 현재 소스와 바이트 동일함을 확인했다. 문서화한 본/루트/콜백/대표/피해 관련 패턴의132개 행을 선정했다. 새 게임 실행·PBO·그래프·물리·운영 설정 변경은 없다.
기존 NOCOLLISION ON/OFF, 부모 연결 실패,5파트 서버 CPU ETL 분석,Drive/Displace 단일 solve 거부 근거를 재사용했다. 같은 시험이나 전체 Ghidra 초기화를 반복하지 않았다.

## 3. 측정 결과
새 런타임 성능값 없음. 정적 감사:15개 PBO,기본 소스18개 일치,132개 선정 호출 행. 새 p50/p95/p99·8/64 회귀·화면 평가는 미실행.
기존5파트 서버 CPU 단일 표본의7242 weighted samples 중 최상위 해석기 추정 범위 leaf1474(20.35%),inclusive5109(70.55%)는 기존 결과다. inclusive를 순수 스크립트 비용이나 이번 후보 절감률로 해석하지 않는다. 기존5→4 비용3쌍 기준 실패는 그대로 유지한다.

## 4. 원인 분석
확인: Ground의 JointWorldPoint·SolveContactsInternal·DriveGround,Traversal의 등반 끝 오차,기존 motion-contract 관측은 Leader의 본을 읽는다. 시야는 agent.Host의 머리를 읽으며 Agent.Initialize(Leader)로 연결된다. 피해 수신자는 spine2를 별도로 읽고 피격/사망 처리에 참여한다.
확인: DGNativeCommand.RunPrePhys는 대표 여부를 검사하기 전에 모든 파트에서 PrePhys_GetTranslation,NativeRootRead/Delta 저장,delta.Length 누적을 수행한다. 실제 GroundBody.Drive는 GroundLeader에만 호출한다. 그 후 모든 관리 파트는 PrePhys_SetTranslation(0)을 실행한다. 현재 핀된 모드 스크립트와 평지 임무에서는 비대표 NativeRootRead/Delta/Distance의 게임플레이 소비자를 찾지 못했다. NativeSeconds/AnimUpdates/NativeRequest는 파트 시계 검사에 쓰이므로 제거 대상이 아니다.
미확인: 스크립트에서 본 읽기가 없다는 사실은 네이티브 피격 형상·애니메이션 평가·복제에 의존성이 없다는 증거가 아니다. 서버 비대표 애니메이션 전체 중단은 정당화되지 않는다. 기존 NOCOLLISION은 참조/위치 계약 실패,부모 연결은 월드 변환 계약 실패였다. 이를 다시 시도하지 않는다.
정정: 앞선 stimulus-source-review.json의 limits 중 'Starting-position variation is observed' 문장은 실제6회 offset0 자료와 맞지 않는다. 원본은 보존하며 이 문장에 근거한 인과 주장은 철회한다. 공개 비용 보고서는 이미 최초 위치 불일치를 지지하지 않는다고 기재했다. 서로 다른 양측 입력 시간식은 확인됐지만 실제 우선순위/비용 영향은 미확인이다.

## 5. 검증 수준
핀된 소스·호출 소유권:진단 성공. 네이티브 비대표 포즈 불필요성:미입증. 아래 국소 후보:NOT_TESTED,아직 구현하지 않음. 기능·시각·반복 비용·최종 수용은 별개다.

## 6. 변경 사항
읽기 전용 감사 스크립트·파생 JSON·보고서만 추가. 게임 소스·PBO·배포·개인키 변경 없음,이번 작업의 신규 서명 대상 없음. 과거 서명 검증 사실을 새 서명 수행으로 표시하지 않는다.

## 7. 원시 증거
server-pose-consumer-review.json에15개 PBO 해시·소스 멤버 해시·선정 행번호/문맥이 있다. 선택 패턴으로 찾은 행이며 모든 네이티브 소비자의 완전 목록은 아니다. DGNativeProbe.c RunPrePhys207–218,Ground239/364/796,Traversal1082,motion-contract38–41 및 기존 layer-review/host-parent-contract1b/server-cpu1 보고서를 연결한다. 원본 로그·PBO·이전 보고서는 덮어쓰지 않았다.

## 8. 잔여 문제
다음 단일 국소 후보는 비대표 관리 파트의 사용되지 않는 루트 이동 조회·누적만 생략하는 opt-in 경로다. 조건은 Owner 존재,GroundBody 존재,GroundLeader=false,DamageReceiver=false,ApplyRootMotion=false이고 retirement 및 원래 대표/피해/비관리 경로는 그대로 둔다. PrePhys_SetTranslation(0),그래프·14클립·파트 시계·물리 본체·지지/충돌·퇴역을 유지한다. 전체 애니메이션 끄기나 갱신 생략이 아니다.
예상 이점은 스크립트→네이티브 getter와 벡터 누적을 줄이는 것뿐이며 실제 CPU 효과는 모른다. 위험은 보이지 않은 관측 소비자와 API 읽기의 미확인 부수효과다. 새 구현 전 비대표 루트 진단 필드는 '미수집'으로 명확히 구분하고 진단 요청 시 원래 경로로 복귀해야 한다. 먼저 기존 하네스의1/8개체 짧은 계약 비교에서 대표 이동·파트 시계·정상 낙하/착지/퇴역 보존과 실제 getter 감소를 확인한다. 실패하면 폐기한다. 작은 비교에서 비용 개선 근거가 없으면64명 장시간 시험으로 확대하지 않는다. 유효할 때만 동일 조건64명 반복 비용 비교와기능 회귀를 분리한다. 기존5파트 기준은 계속 보존한다.

