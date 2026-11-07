# 직접 계산량 감소·파트 소유권 검토

## 1. 작업 목표
64개체 직접 계산량 감소 후보를 기존 연구와 대조하고, 클라이언트 크래시 이후의 서버·클라이언트 소유권 추정을 검증한다.

## 2. 실제 수행
실패한 ANIM-ct64-1-control manifest의 13개 PBO 해시를 실제 파일과 대조했다. 그 안의 비압축 스크립트 33개를 읽어 collision block 생성과 Drive 호출 경로를 확인했다. 현재 Rotate 및 Floors/Walls 소스를 읽고 반복 불변 계산 후보를 검토했다. 게임 실행·새 PBO·물리 정책 변경은 하지 않았다.

## 3. 측정 결과
이번 묶음은 정적 검토이며 p50/p95/p99, CPU 절감, 시각 품질을 새로 측정하지 않았다. 기존 timing-off 8개체 후보는 8/8 단일 기능 완주했으나 대조군은 고지대 milestone 미충족으로 종료되어 쌍 전체 회귀 수용은 아니다. 기존 64 비용 시도는 측정 시작 전 native crash로 유효 비용 표본 0이다. 20ms p95 달성 주장은 없다.

## 4. 원인 분석
확인: 활성 five-part 초기화는 server+multiplayer 조건이며, 표시 파트 5개와 receiver 1개 간 15쌍의 내부 collision block을 만든다. core RunPrePhys는 GroundBody와 GroundLeader가 설정된 경우에만 scripted Drive를 호출한다. 감사한 로드 스크립트에서 클라이언트 별도 block 생성은 발견하지 못했다.
미확인: 네이티브 block 복제, 원격 client command 실행, 네이티브 접촉량, 크래시와의 인과관계. 초기화 코드만으로 client collision 미차단을 선언할 수 없다.
추정 후보: 같은 heading의 회전에서 소스상 삼각함수 표현 16회를 solve당 2회로 줄일 수 있다. 실제 컴파일러 공통식 제거 여부와 비용 비중은 미확인이므로 신규 빌드 없이 후순위 보관한다. 이는 물리 질의·애니메이션 평가 절감 후보가 아니다.

## 5. 검증 수준
진단 성공: packed script/hash 정적 감사 범위에 한정한다. 기능 회귀 통과·반복 성능 검증·시각 최종 수용은 이번 묶음에서 수행하지 않았다. 목표 전체는 미완료다.

## 6. 변경 사항
추가: packed-ownership-audit.json 및 direct-computation-feasibility1.json. 게임 코드·배포 PBO·공용 실행기·검사 성공 조건은 변경하지 않았다. 새 빌드나 새 서명 대상은 없다.

## 7. 원시 증거
기존 ANIM-ct64-1-control manifest와 packed-ownership-audit.json의 PBO/script 해시 및 호출 위치를 연결한다. 직접 계산량 후보는 direct-computation-feasibility1.json에 입력 소스 해시와 미확인 사항을 보관했다. 기존 덤프·로그는 덮어쓰지 않았다. 공개본에는 로컬 경로·덤프·프로세스 정보·개인키를 포함하지 않는다.

## 8. 잔여 문제
64개체 timing-only CPU 비교 미완료, client crash 원인 미확정, control 8개체 milestone 실패 분류 미완료, 전체 기능·시각·성능 최종 수용 미완료. 동일 원인 추정에 기반한 collision 제거와 반복 smoke는 하지 않는다. 전용 측정 시간 가능 여부를 사용자에게 문의했으며 답변 전에는 가용으로 간주하지 않는다. 새로운 후보를 늘리기보다 기존 비교를 완료하는 것이 우선이다.
